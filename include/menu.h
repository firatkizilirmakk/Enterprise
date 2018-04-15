#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro_font.h>
#include "objects.h"

void initMenu(menuT *menu , ALLEGRO_FONT *f);
void updateMenu(menuT *menu,int movement);
void drawMenu(menuT *menu,ALLEGRO_DISPLAY_MODE *dispMode);

#endif
