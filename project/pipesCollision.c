#include "pipesCollision.h"
#include "lcd.h"
#include "delay.h"
#include "main.h"
int numPipes = 4;
int pipeX[4] = {100, 200, 300, 400};
int pipeY[4];
int pipeWidth = 20;
int pipeGap = 50;
int pipeSpeed = 2;

void updatePipes() {
    for (int i = 0; i < numPipes; i++) {
        pipeX[i] -= pipeSpeed;
        if (pipeX[i] + pipeWidth < 0) {
            pipeX[i] = screenWidth;
            pipeY[i] = randomGapHeight();
        }
    }
}
int randomGapHeight() {
    return rand() % (screenHeight - pipeGap);
}
void checkCollisions() {
    for (int i = 0; i < numPipes; i++) {
        if (isColliding(birdX, birdY, birdWidth, birdHeight,
                        pipeX[i], pipeY[i], pipeWidth, pipeHeight)) {
            gameOver = 1;
                        }
    }
    if (birdY <= 0 || birdY + birdHeight >= screenHeight) {
        gameOver = 1;
    }
}

int isColliding(int birdX, int birdY, int birdWidth, int birdHeight,
                int pipeX, int pipeY, int pipeWidth, int pipeHeight) {
    return !(birdX + birdWidth < pipeX ||
             birdX > pipeX + pipeWidth ||
             birdY + birdHeight < pipeY ||
             birdY > pipeY + pipeHeight);
}

