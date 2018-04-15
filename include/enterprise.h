#ifndef ENTERPRISE_H
#define ENTERPRISE_H

#include "objects.h"
#include <allegro5/allegro.h>

typedef enum {CONTINUOUS,MISSILE,LASER,LIGHTENING}enterpriseTypes;
typedef enum {NORMAL,ENT_IDLE,ENT_STEPBACK,
	      ENT_CHASE,
	      ENT_RUNAWAY,ENT_CHASEPRE,ENT_RUNFROMBULL}enterpriseStates;

void initEnterprise(enterpriseT *ship,ALLEGRO_BITMAP *image,
		    ALLEGRO_DISPLAY_MODE *dispMode,int type);
void drawEnterprise(enterpriseT *ship);
void moveEnterpriseUp(enterpriseT *ship);
void moveEnterpriseDown(enterpriseT *ship,
			ALLEGRO_DISPLAY_MODE *dispMode);
void moveEnterpriseLeft(enterpriseT *ship);
void moveEnterpriseRight(enterpriseT *ship);
void resetEnterpriseFrame(enterpriseT *ship , int position);
void collideEnterprise(enterpriseT *ship ,
		       bulletT enemyBullets[] , int bSize);
enemyT * nearestEnemyToEnterprise(enterpriseT *ship,
				  enemyT enemies[],int eSize);
presentT *  nearestPresent(enterpriseT *ship,
			   presentT *pre,int pSize);
bulletT * comingBullet(enterpriseT *ship,bulletT *enemyBullets,int bSize);
void updateAutoEnterprise(enterpriseT *ship,enemyT enemies[],
			  int eSize,presentT *pre,int pSize,
			  bulletT *enemyBullets,int enSize);
void checkEnterpriseState(enterpriseT *ship, int newState);
void goToBase(enterpriseT *ship);
#endif
