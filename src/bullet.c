#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "../include/bullet.h"
#include "../include/enterprise.h"
#include "../include/matfuncs.h"
#include "../include/explosion.h"
#include "../include/main.h"

void initEnterpriseBullet(bulletT bullets[] ,int bSize,
			  ALLEGRO_BITMAP *images[],
			  ALLEGRO_SAMPLE *samples[])
{
  int i;
  for(i = 0 ; i < bSize ; ++i)
    {
      bullets[i].ID = BULLET;
      bullets[i].alive = 0;
      bullets[i].routed = 0;
      bullets[i].diffX = -1;
      bullets[i].diffY = -1;
      bullets[i].count = 0;
      bullets[i].time = 0;
      bullets[i].vX = 3 + rand() % 2;
      bullets[i].vY = 3 + rand() % 2;
      bullets[i].enemy = (enemyT *)malloc(sizeof(enemyT));
      bullets[i].enemy -> alive = 0;
      if(i < 10)
	{
	  bullets[i].type = CONTINUOUS;
	  bullets[i].damage = 7;
	  bullets[i].speed = 8;
	  bullets[i].bX = 1;
	  bullets[i].bY = 1;
	  bullets[i].image = NULL;
	  bullets[i].sound = samples[0];
	}
      else if(i < 20)
	{
	  bullets[i].type = MISSILE;
	  bullets[i].damage = 14;
	  bullets[i].speed = 6;
	  bullets[i].bX = 15;
	  bullets[i].bY = 17;
	  bullets[i].image = images[0];
	  bullets[i].sound = samples[1];

	  bullets[i].curNumFrame = 0;
	  bullets[i].maxNumFrame = 2;
	  bullets[i].frameWidth = 75;
	  bullets[i].frameHeight = 150;
	}
      else if(i < 30)
	{
	  bullets[i].type = LASER;
	  bullets[i].damage = 10;
	  bullets[i].speed = 9;
	  bullets[i].bX = 15;
	  bullets[i].bY = 8;
	  bullets[i].image = images[1];
	  bullets[i].sound = samples[2];

	  bullets[i].curNumFrame = 0;
	  bullets[i].maxNumFrame = 3;
	  bullets[i].frameWidth = 80;
	  bullets[i].frameHeight = 13;
	}
      else if(i < 40)
	{
	  bullets[i].type = LIGHTENING;
	  bullets[i].damage = 18;
	  bullets[i].speed = 10;
	  bullets[i].bX = 2;
	  bullets[i].bY = 2;
	  bullets[i].image = images[2];
	  bullets[i].sound = samples[3];
	}
    }
}
/*draw the bullet according to enterprise type*/
void drawEnterpriseBullet(bulletT bullets[],int bSize)
{
  int i;
  int sx;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(bullets[i].alive == 1)
	{
	  if(bullets[i].type == CONTINUOUS)
	    {
	      al_draw_filled_circle(bullets[i].x,
				    bullets[i].y,
				    3 ,
				    al_map_rgb(255, 255, 255));
	    }
	  else
	    {
	      switch(bullets[i].type)
		{
		case MISSILE:
		case LASER:
		  if(bullets[i].time >= 25)
		    bullets[i].curNumFrame = 1;
		  sx = bullets[i].curNumFrame *
		    bullets[i].frameWidth;
		  al_draw_bitmap_region(bullets[i].image,sx,0,
					bullets[i].frameWidth,
					bullets[i].frameHeight,
					bullets[i].x -
					bullets[i].frameWidth / 2,
					bullets[i].y -
					bullets[i].frameHeight / 2,
					0);
		  break;
		case LIGHTENING:
		  al_draw_bitmap(bullets[i].image,
				 bullets[i].x,bullets[i].y,0);
		}
	    }
	}
    }
}
/*fire the bullet with respect to its type*/
void fireEnterpriseBullet(bulletT bullets[],int bSize,
			  enterpriseT *ship,int entType)
{
  int i,count = 0;
  for(i = 0 ; i < bSize ; ++i)
    {
      /*if it is not fired,then fire*/
      if(bullets[i].alive == 0 && bullets[i].type == entType)
	{
	  bullets[i].alive = 1;
	  bullets[i].routed = 1;
	  bullets[i].diffX = -1;
	  bullets[i].diffY = -1;
	  bullets[i].enemySpeed = -1;
	  bullets[i].time = 0;
	  bullets[i].vX = 3 + rand() % 2;
	  bullets[i].vY = 3 + rand() % 2;
	  /* -1 are flags*/
	  switch (entType)
	    {
	    case CONTINUOUS:
	      bullets[i].x = ship -> x + 35;
	      bullets[i].y = ship -> y;
	      al_play_sample(bullets[i].sound,0.75,0,1.25,
			     ALLEGRO_PLAYMODE_ONCE,NULL);
	      break;
	    case LIGHTENING:
	      bullets[i].x = ship -> x + 30;
	      bullets[i].y = ship -> y - 3;
	      al_play_sample(bullets[i].sound,1,0,1.25,
			     ALLEGRO_PLAYMODE_ONCE,NULL);
	      break;
	    case LASER:
	      bullets[i].x = ship -> x + 30;
	      bullets[i].y = ship -> y - 3 +
		(count == 1 ? 10 : 
		 (count == 2 ? -10 : 0));
	      bullets[i].count = count;
	      count +=1;
	      al_play_sample(bullets[i].sound,1,0,1.25,
			     ALLEGRO_PLAYMODE_ONCE,NULL);
	      break;
	    case MISSILE:
	      bullets[i].x = ship -> x + 30;
	      bullets[i].y = ship -> y + 1;
	      al_play_sample(bullets[i].sound,0.75,0,1.60,
			     ALLEGRO_PLAYMODE_ONCE,NULL);
	      break;
	      /*do for other cases*/
	    }
	  if(bullets[i].type == LASER && count == 3)
	    break;
	  else if(bullets[i].type == LIGHTENING ||
		  bullets[i].type == MISSILE ||
		  bullets[i].type == CONTINUOUS)
	    break;
	}
    }
}
/*update bullet's positions with respect to its type*/
void updateEnterpriseBullet(bulletT bullets[],int bSize,
			    enemyT enemies[],int eSize,
			    ALLEGRO_DISPLAY_MODE *dispMode)
{
  int i;
  enemyT *tmp;
  float dX,dY,x,y,dist;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(bullets[i].alive == 1)
	{
	  bullets[i].time +=1;
	  switch (bullets[i].type)
	    {
	    case CONTINUOUS:
	      bullets[i].x += bullets[i].speed;
	      break;
	      /*missile shoot to the nearest enemy*/
	      /*lightening tries to shoot the enemy 
	      **with lowest health
	      */
	    case MISSILE:
	    case LIGHTENING:
	      if(bullets[i].routed == 1)
		{
		  if(bullets[i].type == MISSILE)
		    tmp = nearestEnemy(&bullets[i],enemies,eSize);
		  else if(bullets[i].type == LIGHTENING)
		    tmp = lowestHealthEnemy(enemies,eSize);
		  /*nearest enemy alive,there is at least an enemy*/
		  if(tmp -> alive == 1 &&
		     tmp -> x > bullets[i].x + 50)
		    {
		      bullets[i].diffX = tmp -> x;
		      bullets[i].diffY = tmp -> y;
		      bullets[i].enemySpeed = tmp -> speed;
		      bullets[i].routed = 0;
		      bullets[i].enemy = tmp;
		    }
		}
	      /*go through enemies*/
	      if(bullets[i].diffX != -1 && bullets[i].diffY != -1 &&
		 bullets[i].enemy -> alive == 1)
		{
		  dX = bullets[i].diffX - bullets[i].x;
		  dY = bullets[i].diffY - bullets[i].y;
		  dist = sqrt(dX * dX + dY * dY);
		  x = (dX / dist) * 5;
		  y = (dY / dist) * 5;
		  bullets[i].vX += x;
		  bullets[i].vY += y;
		  bullets[i].x += bullets[i].vX / 15;
		  bullets[i].y += bullets[i].vY / 15;
		}
	      else if(bullets[i].enemy -> alive == 0)
		{
		  dX = WIDTH - bullets[i].x;
		  dY = (HEIGHT / 2) - bullets[i].y;
		  dist = sqrt(dX * dX + dY * dY);
		  x = (dX / dist) * 5;
		  y = (dY / dist) * 5;
		  bullets[i].vX += x;
		  bullets[i].vY += y;
		  bullets[i].x += bullets[i].vX / 15;
		  bullets[i].y += bullets[i].vY / 15;
		}
	      /* else if(bullets[i].time > 50) */
	      /* 	{ */
	      /* 	  angle = angleToTarget(WIDTH,bullets[i].x, */
	      /* 				HEIGHT / 2,bullets[i].y); */
	      /* 	  bullets[i].x += (2 * cos(angle)); */
	      /* 	  bullets[i].y += (2 * sin(angle)); */
	      /* 	}  */
	      break;
	    case LASER:
	      bullets[i].y += (bullets[i].count == 1 ? 2 :
			       (bullets[i].count == 2 ? -2 : 0));
	      bullets[i].x += bullets[i].speed;
	      break;
	    }
	  if(bullets[i].x > WIDTH ||
	     bullets[i].y < 0 ||
	     bullets[i].y > HEIGHT)
	    bullets[i].alive = 0;
	}
    }
}

/*bounding box collision detection
 *check for whether bullets collide with enemies
 **/
void bulletCollision(bulletT *bullets,int bSize,
		     enemyT *enemies,int eSize,
		     explosionT *explosion,int exSize,
		     int *shipScore)
{
  int i,j;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  for(j = 0; j < bSize ; ++j)
	    {
	      if(bullets[j].alive == 1)
		{
		  if((bullets[j].x + bullets[j].bX) >
		     (enemies[i].x - enemies[i].bX) &&
		     (bullets[j].x - bullets[j].bX) <
		     (enemies[i].x + enemies[i].bX) &&
		     (bullets[j].y + bullets[j].bY) >
		     (enemies[i].y - enemies[i].bY) &&
		     (bullets[j].y - bullets[j].bY) <
		     (enemies[i].y + enemies[i].bY))
		    {
		      if(bullets[j].type == LASER)
			enemies[i].loss = 0;
		      else
			enemies[i].loss -= bullets[j].damage;
		      if(enemies[i].loss <= 0)
			{
			  enemies[i].alive = 0;
			  startExplosion(explosion,exSize,
					 enemies[i].x,
					 enemies[i].y);
			  *shipScore += enemies[i].lives / 7;
			}
		      bullets[j].alive = 0;
		    }
		}
	    }
	}
    }
}

/*find the nearest enemy and return that whole structure*/
enemyT *nearestEnemy(bulletT *bullet,enemyT enemies[],int eSize)
{
  enemyT *tmp;
  int i,x = -1;
  int minDist = 9999;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  if(distance(enemies[i].x,enemies[i].y,
		      bullet -> x,bullet -> y) < minDist)
	    {
	      minDist = distance(enemies[i].x,enemies[i].y,
				 bullet -> x,bullet -> y);
	      x = i;
	    }
	}
    }
  /*if there is not any alive enemy*/
  if(x == -1)
    {
      tmp = (enemyT *)malloc(sizeof(enemyT));
      tmp -> alive = 0;
    }
  else
    tmp = &(enemies[x]);
  return tmp;
}
/*return the enemy type which has the lowest lives*/
enemyT * lowestHealthEnemy(enemyT enemies[],int eSize)
{
  enemyT *tmp = NULL;
  int i,x = -1;
  int minHealth = 999;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1 && enemies[i].lives < minHealth)
	{
	  x = i;
	  minHealth = enemies[i].lives;
	}
    }
  if(x == -1)
    {
      tmp = (enemyT * )malloc(sizeof(enemyT));
      tmp -> alive = 0;
    }
  else
    {
      tmp = &(enemies[x]);
    }
  return tmp;
}
