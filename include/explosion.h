#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <allegro5/allegro.h>
#include "objects.h"

void initExplosion(explosionT *explosion,int eSize,
		   ALLEGRO_BITMAP *image,
		   ALLEGRO_SAMPLE *sample);
void startExplosion(explosionT *explosion,int eSize,int x,int y);
void updateExplosion(explosionT *explosion,int eSize);
void drawExplosion(explosionT *explosion,int eSize);
#endif
