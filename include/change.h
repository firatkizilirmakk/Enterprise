#ifndef CHANGE_H
#define CHANGE_H
#include <allegro5/allegro.h>
#include "../include/objects.h"

/*when the enterprise catches a present and gets changed the
type show this passing image*/
void initChange(changeT *change,ALLEGRO_BITMAP *image);
void updateDrawChange(changeT *change,enterpriseT *ship);
#endif
