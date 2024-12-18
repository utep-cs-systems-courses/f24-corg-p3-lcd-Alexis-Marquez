#ifndef MAIN_H
#define MAIN_H


#define LED BIT6/* note that bit zero req'd for display */
#define SWITCHES 15
int gameOver;
int jumpForce;
int redrawScreen;
int switches;
int birdX;
int birdY;
int birdWidth;
int birdHeight;
int gravity;
int pause;
int mute;
void jump();
void reset();
void update_bird();
void switch_interrupt_handler();
void draw_screen_and_sound();
#endif //MAIN_H
