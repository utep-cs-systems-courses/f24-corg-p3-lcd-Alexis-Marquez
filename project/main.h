#ifndef MAIN_H
#define MAIN_H


#define LED BIT6/* note that bit zero req'd for display */
#define SWITCHES 15
int gameOver;
int redrawScreen;
int switches;
int birdX;
int birdY;
int birdWidth;
int birdHeight;
int gravity;
int pause;
void jump();
void reset();
#endif //MAIN_H
