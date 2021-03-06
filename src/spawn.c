/*during the game spawn presents which provide ENTERPRISE 
more facilities such as missile gun,lightening gun,laser gun
*/

#include "../include/spawn.h"
#include "../include/enterprise.h"
#include "../include/main.h"
#include "../include/change.h"
void initPresent(presentT *pre , ALLEGRO_BITMAP *image,
		 int entType,ALLEGRO_SAMPLE *sample)
{
  pre -> ID = PRESENT;
  pre -> type = entType;
  pre -> speed = 5;
  pre -> bX = 15;
  pre -> bY = 15;
  pre -> alive = 0;
  pre -> time = 0;
  pre -> image = image;
  pre -> sound = sample;
  pre -> caughtByEnt = 0;
}
/*start the present*/
void startPresent(presentT pre[],int pSize,
		  gameT *gT,ALLEGRO_DISPLAY_MODE *dispMode)
{
  int i;
  /*TODO fix values*/
  int prob = 400 * gT -> difficulty;
  for(i = 0 ; i < pSize ; ++i)
    {
      if(pre[i].alive == 0)
	{
	  if(rand() % prob == 0)
	    {
	      pre[i].x = WIDTH;
	      pre[i].y = rand() % (HEIGHT - 80) + 20;
	      pre[i].alive = 1;
	      pre[i].time = 0;
	      pre[i].caughtByEnt = 0;
	      break;
	    }
	}
    }
}
/*update the present's x and y positions and
arrange ship's type with respect to present 
caught by enterprise*/
void updatePresent(presentT pre[],int pSize,
		   enterpriseT *ship,int difficulty)
{
  int i;
  for(i = 0 ; i < pSize ; ++i)
    {
      if(pre[i].alive == 1)
	{
	  pre[i].x -= pre[i].speed;
	  if(pre[i].x < 0)
	    pre[i].alive = 0;
	}
      if(ship -> caughtPresent == 1)
	{
	  ship -> presentTime +=1;
	  /*TODO time may be changed*/
	  if(ship -> presentTime >= 1000 + 100 * difficulty)
	    {
	      ship -> type = CONTINUOUS;
	      ship -> presentTime = 0;
	      ship -> caughtPresent = 0;
	    }
	}
    }
}
void drawPresent(presentT pre[] , int pSize)
{
  int i;
  for(i = 0 ; i < pSize ; ++i)
    {
      if(pre[i].alive == 1)
	al_draw_bitmap(pre[i].image,pre[i].x,pre[i].y,0);
    }
}
/*detect the collision between enterprise and present*/
void preCollision(presentT pre[],int pSize,
		  enterpriseT *ship,changeT *change)
{
  int i;
  for(i = 0 ; i < pSize ; ++i)
    {
      if(pre[i].alive == 1)
	{
	  if(pre[i].x < (ship -> x + ship -> bX) &&
	     pre[i].x > (ship -> x - ship -> bX) &&
	     pre[i].y < (ship -> y + ship -> bY) &&
	     pre[i].y > (ship -> y - ship -> bY))
	    {
	      pre[i].alive = 0;
	      ship -> presentTime = 0;
	      ship -> caughtPresent = 1;
	      ship -> type = pre[i].type;
	      al_play_sample(pre[i].sound,1,0,1,
			     ALLEGRO_PLAYMODE_ONCE,0);
	      change -> alive = 1;
	      change -> x = ship -> x;
	      change -> y = ship -> y;
	      /*enterprise can shoot this type of bullet*/
	    }
	}
    }
}
