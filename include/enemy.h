#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include "objects.h"
#include "main.h"

typedef enum {STATIC,STATICF,STATICM,DYNAMIC,DYNAMICF}enemy_types;
typedef enum {ENMY_NORMAL,ENMY_CHASE,
	      ENMY_IDLE,ENMY_STEPBACK,ENMY_RUNAWAY};	      

void initEnemy(enemyT enemies[] , int eSize,
	       ALLEGRO_BITMAP *images[]);
void drawEnemy(enemyT enemies[] , int eSize);
void startEnemy(enemyT enemies[] , int eSize ,
	       gameT *gT);
void updateEnemy(enemyT enemies[] , int eSize ,
		 enterpriseT *ship,
		 bulletT *bullets,
		 int bSize);
int isThereAnotherObject(enemyT enemies[],int eSize,int x,int y);
void initEnemyBullet(bulletT bullets[],int bSize);
void fireEnemyBullet(bulletT bullets[],int bSize,
		     enemyT enemies[],int eSize);
void updateEnemyBullet(bulletT bullets[],int bSize,
		       enterpriseT *ship);
void drawEnemyBullet(bulletT bullets[],int bSize);
void autoEnemy(enterpriseT *ship,enemyT *enemy,
	       bulletT *bullets,int bSize);
bulletT * bulletToEnemy(enemyT *enemy,bulletT *bullets,int bSize);
void enterpriseCollision(enterpriseT *ship,enemyT *enemies,int eSize
			 ,explosionT *explosions,int exSize);
#endif
