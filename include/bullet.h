#ifndef BULLET_H
#define BULLET_H

#include "objects.h"

void initEnterpriseBullet(bulletT bullets[] ,int bSize,
			  ALLEGRO_BITMAP *images[],
			  ALLEGRO_SAMPLE *samples[]);
void drawEnterpriseBullet(bulletT bullets[],int bSize);
void fireEnterpriseBullet(bulletT bullets[],int bSize,
			  enterpriseT *ship,int entType);
void updateEnterpriseBullet(bulletT bullets[],int bSize,
			    enemyT enemies[],int eSize,
			    ALLEGRO_DISPLAY_MODE *dispMode);
void bulletCollision(bulletT *bullets,int bSize,
		     enemyT *enemies,int eSize,
		     explosionT *explosion,int exSize,
		     int *shipScore);
enemyT * nearestEnemy(bulletT *bullet,enemyT enemies[],int eSize);
enemyT * lowestHealthEnemy(enemyT enemies[],int eSize);
#endif
