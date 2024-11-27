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
    switches = ~p2val & SWITCHES;
}

void
draw_bird(int col, int row, unsigned short color)
{
    fillRectangle(col-1, row-1, birdWidth, birdHeight, color);
}
void draw_pipe(int x, int y, int gap, unsigned short color){
    fillRectangle(x, 0, pipeWidth, y, color);
    fillRectangle(x, y+gap, pipeWidth, screenHeight, color);
}

void draw_pipes(){
     for(int i=0; i < numPipes; i++){
           draw_pipe(pipeX[i], pipeY[i], pipeGap[i], COLOR_GREEN);
       }
  }

void jump(){
    birdY -= gravity;
}

void update_bird(){
      birdY -= gravity;
  }

void reset(){
  pipeX[0] = 100;
  pipeX[1] = screenWidth;
  pipeX[2] = screenWidth;
}

int main(){
    P1DIR |= LED;		/**< Green led on when CPU on */
    P1OUT |= LED;
    configureClocks();
    lcd_init();
    switch_init();

    enableWDTInterrupts();      /**< enable periodic interrupt */
    or_sr(0x8);	              /**< GIE (enable interrupts) */

    clearScreen(COLOR_BLUE);
    while (!gameOver) {
        or_sr(0x10);
    }
 }
void wdt_c_handler()
{
    static int secCount = 0;
    static int step = 0;
    secCount ++;
    if (secCount >= 25) {		/* 10/sec */
        if(!pause){
         clearScreen(COLOR_BLUE);
         update_bird();
         updatePipes();
	     draw_bird(birdX,birdY,COLOR_YELLOW);
         draw_pipes();
         checkCollision();
         if (switches & SW2) jump();
        }
        if (switches & SW3) pause ^= 1;
        if (switches & SW1) reset();
        if (step <= 30)
            step ++;
        else
              step = 0;
        secCount = 0;
        if (switches & SW4) return;
    }
}
