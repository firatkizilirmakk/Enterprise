#ifndef SPAWN_H
#define SPAWN_H

#include <allegro5/allegro.h>
#include "objects.h"
#include "main.h"

void initPresent(presentT *pre , ALLEGRO_BITMAP *image,
		 int entType,ALLEGRO_SAMPLE *sample);
void startPresent(presentT pre[],int pSize,
		    gameT *gT,ALLEGRO_DISPLAY_MODE *dispMode);
void updatePresent(presentT pre[],int pSize,
		   enterpriseT *ship,int difficulty);
void drawPresent(presentT pre[] , int pSize);
void preCollision(presentT pre[],int pSize,
		  enterpriseT *ship,changeT *change);
#endif
