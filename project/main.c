#include "main.h"
#include "pipesCollision.h"
#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

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
  birdX = 100;
  birdY = 100;
  gameOver = 0;
  birdWidth = 10;
  birdHeight = 10;
  gravity = 1;
  }

void

switch_interrupt_handler()

{
    char p2val = switch_update_interrupt_sense();

    if (!(p2val & SW1)) {  // Check if Switch 1 is pressed
        jump();
    } else if (!(p2val & SW2)) {  // Check if Switch 2 is pressed
        pause ^= 1;
    } else if (!(p2val & SW3)) {  // Check if Switch 3 is pressed
        reset();
    } else if (!(p2val & SW4)) {  // Check if Switch 4 is pressed
    }
    switch_state_changed = 1;
}

void
draw_bird(int col, int row, unsigned short color)
{
    fillRectangle(col, row, birdWidth, birdHeight, color);
    fillRectangle(col, row+gravity, birdWidth, birdHeight, COLOR_BLUE);
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

void jump(){
    birdY -= gravity;
    fillRectangle(birdX, birdY-gravity, birdWidth, birdHeight, COLOR_BLUE);
}

void update_bird(){
      birdY -= gravity;
  }

void reset(){
  pipeX[0] = screenWidth-50;
  pipeX[1] = screenWidth;
  pipeX[2] = screenWidth+50;
}

int main(){
    P1DIR |= LED;		/**< Green led on when CPU on */
    P1OUT |= LED;
    configureClocks();
    lcd_init();
    switch_init();
    clearScreen(COLOR_BLUE);
    enableWDTInterrupts();      /**< enable periodic interrupt */
    or_sr(0x18);	              /**< GIE (enable interrupts) */
 }
void wdt_c_handler()
{
    static int secCount = 0;
    secCount ++;
    if (secCount >= 25/3) {		/* 10/sec */
     if(!pause){
       //update_bird();
         updatePipes();
         draw_bird(100,100,COLOR_BLACK);
	     draw_pipes();
         checkCollision();
        }
        secCount = 0;
    }
}
