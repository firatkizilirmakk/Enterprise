#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "objects.h"
#include <allegro5/allegro.h>

void initBackground(backgroundT *back, float x, float y, float velx,
		    float vely, int width, int height, int dirX,
		    int dirY, ALLEGRO_BITMAP *image);
void updateBackground(backgroundT *bg);
void drawBackground(backgroundT *bg,ALLEGRO_DISPLAY_MODE *dispMode);


#endif
