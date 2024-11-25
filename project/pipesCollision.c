#include "pipesCollision.h"
#include "main.h"
#include "lcdutils.h"
#include "lcddraw.h"

int numPipes = 3;
int pipeX[3] = {100, 200, 300};
int pipeY[3] = {20, 30, 40};
int pipeWidth = 20;
int pipeGap[3] = {15, 50, 60};
int pipeSpeed = 2;

void updatePipes() {
    for (int i = 0; i < numPipes; i++) {
        pipeX[i] -= pipeSpeed;
        if (pipeX[i] + pipeWidth < 0) {
            pipeX[i] = screenWidth;
            pipeGap[i] = randomGapHeight();
        }
    }
}
int randomGapHeight() {
  return 20+ rand() % (screenHeight-19);
}
void checkCollision() {
    for (int i = 0; i < numPipes; i++) {
        if (isColliding(birdX, birdY, birdWidth, birdHeight,
                        pipeX[i], pipeY[i], pipeWidth, pipeGap[i])) {
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
