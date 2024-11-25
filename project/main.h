#ifndef MAIN_H
#define MAIN_H


#define LED BIT6/* note that bit zero req'd for display */
#define SWITCHES 15
int gameOver = 0;
int redrawScreen = 0;
int switches;
int birdX;
int birdY;
int birdWidth;
int birdHeight;
int pause = 0;
void jump();
void reset();
#endif //MAIN_H
