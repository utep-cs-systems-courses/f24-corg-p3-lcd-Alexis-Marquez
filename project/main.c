#include "main.h"
#include "pipesCollision.h"
#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

#define LED BIT6

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15
int tempo;

static char
switch_update_interrupt_sense()
{
    char p2val = P2IN;
    /* update switch interrupt to detect changes from current buttons */
    P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
    P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
    return p2val;
}

void
switch_init()			/* setup switch */
{
    P2REN |= SWITCHES;		/* enables resistors for switches */
    P2IE |= SWITCHES;		/* enable interrupts from switches */
    P2OUT |= SWITCHES;		/* pull-ups for switches */
    P2DIR &= ~SWITCHES;		/* set switches' bits for input */
    switch_update_interrupt_sense();
}

void game_init(){
  pause = 0;
  tempo = 0;
  birdX = screenWidth/4;
  birdY = screenHeight/2;
  gameOver = 0;
  birdWidth = 5;
  birdHeight = 5;
  gravity = 3;
  jumpForce = 15;
  mute = 0;
  }

void

switch_interrupt_handler()

{
    char p2val = switch_update_interrupt_sense();

    if (!(p2val & SW1)) {  // Check if Switch 1 is pressed
        jump();
    } else if (!(p2val & SW2)) {  // Check if Switch 2 is pressed
        pause ^= 1;
	clearScreen(COLOR_BLUE);
    } else if (!(p2val & SW3)) {  // Check if Switch 3 is pressed
        reset();
    } else if (!(p2val & SW4)) {  // Check if Switch 4 is pressed
      buzzer_set_period(0);
      mute ^= 1;
    }
}

void
draw_bird(int col, int row, unsigned short color)
{
    fillRectangle(col, row, birdWidth, birdHeight, color);
}
void draw_pipe(int x, int y, int gap, unsigned short color){
    fillRectangle(x, 0, pipeWidth, y, color);
    fillRectangle(x, y+gap, pipeWidth, screenHeight, color);
    fillRectangle(x+pipeWidth, 0, pipeSpeed, screenHeight, COLOR_BLUE);
}

void draw_pipes(){
     for(int i=0; i < numPipes; i++){
       if(pipeX[i]<screenWidth)
           draw_pipe(pipeX[i], pipeY[i], pipeGap[i], COLOR_GREEN);
       }
  }

//void jump(){
//    birdY -= jumpForce;
//    fillRectangle(birdX, birdY+jumpForce, birdWidth, birdHeight, COLOR_BLUE);
//    if(!mute){buzzer_set_period(0); buzzer_set_period(3817);}
//}

void update_bird(){
      birdY += gravity;
      fillRectangle(birdX, birdY - gravity, birdWidth, birdHeight, COLOR_BLUE);
  }

void reset(){
  clearScreen(COLOR_BLUE);
  game_init();
  pipeX[0] = screenWidth-50;
  pipeX[1] = screenWidth;
  pipeX[2] = screenWidth+50;
}

void playSong(){
  if(tempo>=1 & tempo < 25) buzzer_set_period(6810);
  else if(tempo>=25 & tempo < 50) buzzer_set_period(6067);
  else if(tempo>=50 & tempo < 75) buzzer_set_period(5406);
  else if(tempo>=75) buzzer_set_period(5102);
  }

int main(){
    P1DIR |= LED;		/**< Green led on when CPU on */
    P1OUT |= LED;
    configureClocks();
    lcd_init();
    switch_init();
    game_init();
    buzzer_init();
    clearScreen(COLOR_BLUE);

    enableWDTInterrupts();      /**< enable periodic interrupt */
    or_sr(0x8);

    while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      draw_screen_and_sound();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
 }
void wdt_c_handler()
{
    static int secCount = 0;
    secCount ++;
    tempo++;
    if (secCount >= 25/3) {
      /* 30/sec */
	   draw_screen_and_sound();
       secCount = 0;
    }
    if(tempo>=100){tempo=0;}
}

void draw_screen_and_sound(){
   if(!gameOver){
	if(!pause){
	  if(!mute){
	    playSong(tempo);}
         update_bird();
         updatePipes();
         draw_bird(birdX,birdY,COLOR_YELLOW);
	     draw_pipes();
         checkCollision();
        }
	if(pause){
	  buzzer_set_period(0);
	  drawString5x7(screenWidth/2, screenHeight/2, "Pause", COLOR_YELLOW, COLOR_BLUE);
	  }
      }
      else{
	pause = 1;
	buzzer_set_period(0);
	drawString5x7(screenWidth/2, screenHeight/2, "GAME OVER", COLOR_RED, COLOR_BLUE);
      }
  }