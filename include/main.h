#ifndef MAIN_H
#define MAIN_H

#include "objects.h"

#define FPS 60
#define WIDTH 1024
#define HEIGHT 600

typedef enum {UP,DOWN,LEFT,RIGHT,SPACE,ENTER,AUTO,ESC,QUIT}KEYS;
typedef enum {MENU,PLAY,PAUSE,OVER,HIGHSCORE,EXIT}GAME_STATES;
typedef struct
{
  int difficulty;/*from 0 to 10*/
  int shipLives;
  int topDiff;
  int numBullets,
    numEnemyBullets,
    numEnemies,
    numExplosions;
  int time;/*time to arrange difficulty*/
}gameT;

void switchEventType(ALLEGRO_EVENT *event ,int keys[],
		     int type ,int *keyPressed);
void callMovement(int keys[],enterpriseT *ship,
		  ALLEGRO_DISPLAY_MODE *dispMode);
void initGameT(gameT *g);
void changeDifficulty(gameT *g);
void rearrangeObjects(bulletT *enemyBullets[],gameT *gT);
void drawStatistic(int shipScore,int shipLives,int shipState,
		   ALLEGRO_BITMAP *liveImage,
		   ALLEGRO_BITMAP *autoImage,
		   ALLEGRO_FONT *font);
void arrangeMenu(menuT *menu,int keys[],int *gameState);
#endif
