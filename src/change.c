#include "../include/change.h"

void initChange(changeT *change,ALLEGRO_BITMAP *image)
{
  change -> alive = 0;
  change -> curNumFrame = 0;
  change -> frameNumCount = 0;
  change -> maxNumFrame = 3;
  change -> frameNumDelay = 5;  
  change -> frameWidth = 80;
  change -> frameHeight = 80;
  change -> time = 0;
  change -> image = image;
}
/*when the present is caught by enterprise draw change effect*/
void updateDrawChange(changeT *change,enterpriseT *ship)
{
  int sx;
  if(change -> alive == 1)
    {
      change -> frameNumCount +=1;
      if(change -> frameNumCount >= change -> frameNumDelay)
	{
	  change -> curNumFrame +=1;
	  if(change -> curNumFrame >= change -> maxNumFrame)
	    {
	      change -> curNumFrame = 0;
	      change -> alive = 0;
	    }
	  change -> frameNumCount = 0;
	}
      sx = change -> curNumFrame * change -> frameWidth;
      al_draw_bitmap_region(change -> image,sx,0,
			    change -> frameWidth,
			    change -> frameHeight,
			    ship -> x - change -> frameWidth / 2,
			    ship -> y - change -> frameHeight / 2,
			    0);
    }
}
