#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "../include/enterprise.h"
#include "../include/matfuncs.h"
#include "../include/main.h"
#include "../include/enemy.h"
/*type parameter is not needed anymore*/
void initEnterprise(enterpriseT *ship,ALLEGRO_BITMAP *image,
		    ALLEGRO_DISPLAY_MODE *dispMode,int type)
{
  /*object type undependent variables*/
  ship -> ID = ENTERPRISE;
  ship -> type = CONTINUOUS;/*initial type*/
  ship -> x = 50;
  ship -> y = HEIGHT / 2;
  ship -> score = 0;
  ship -> baseX = 50;
  ship -> baseY = HEIGHT / 2;
  ship -> time = 0;
  ship -> presentTime = 0;
  ship -> caughtPresent = 0;
  ship -> state = NORMAL;
  ship -> target = malloc(sizeof(enemyT));
  ship -> targetP = malloc(sizeof(presentT));
  ship -> comingBullet = malloc(sizeof(bulletT));
  ship -> target -> alive = 0;
  ship -> target -> x = -1;
  ship -> target -> y = -1;
  ship -> targetP -> alive = 0;
  ship -> targetP -> x = -1;
  ship -> targetP -> y = -1;
  ship -> comingBullet -> alive = 0;
  
  ship -> bX = 38;
  ship -> bY = 36;
  ship -> lives = 15;
  ship -> speed = 5;
  /*object's bitmap variables*/
  ship -> curNumFrame = 1;
  ship -> frameNumCount = 0;
  ship -> frameWidth = 80;
  ship -> frameHeight = 70;
  ship -> spriteColumns = 6;
  ship -> spriteRows = 0;
  ship -> image = image;
}
/*draw the enterprise with respect to its type
there are four different enterprise types and images*/
void drawEnterprise(enterpriseT *ship)
{
  switch(ship -> type)
    {
    case CONTINUOUS:
      ship -> curNumFrame = 1;
      break;
    case MISSILE:
      ship -> curNumFrame = 3;
      break;
    case LIGHTENING:
      ship -> curNumFrame = 4;
      break;
    case LASER:
      ship -> curNumFrame = 2;
      break;
    }
  /*from where to start draw bitmap*/
  int sY = ship -> spriteRows * ship -> frameHeight;
  int sX = (ship -> curNumFrame %
	    ship -> spriteColumns) * ship -> frameWidth;
  al_draw_bitmap_region(ship -> image,
  			sX,
  			sY,
  			ship -> frameWidth,
  			ship -> frameHeight,
  			ship -> x - ship -> frameWidth / 2,
  			ship -> y - ship -> frameHeight / 2,
  			0);
}
void moveEnterpriseUp(enterpriseT *ship)
{
  ship -> spriteRows = 1;
  ship -> y -= ship -> speed;
  if(ship -> y < 0)
    ship -> y = 0;
}
void moveEnterpriseDown(enterpriseT *ship,
			ALLEGRO_DISPLAY_MODE *dispMode)
{
  ship -> spriteRows = 2;
  ship -> y += ship -> speed;
  if(ship -> y >= HEIGHT - ship -> bY / 2)
    ship -> y = HEIGHT - ship -> bY / 2;
}
void moveEnterpriseLeft(enterpriseT *ship)
{
  /* ship -> curNumFrame = 0; */
  ship -> x -= ship -> speed;
  if(ship -> x < 35)
    ship -> x = 35;
}
void moveEnterpriseRight(enterpriseT *ship)
{
  /* ship -> curNumFrame = 1; */
  ship -> x += ship -> speed;
  if(ship -> x > 400)/*can not go far away*/
    ship -> x = 400;
}

void resetEnterpriseFrame(enterpriseT *ship , int position)
{
  if(position == 1)
    ship -> spriteRows = 0;
  else if(position == 0)
    ship -> curNumFrame = 0;
}

/*collide enterprise with a bullet*/
void collideEnterprise(enterpriseT *ship ,
		       bulletT enemyBullets[] , int bSize)
{
  int i;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(enemyBullets[i].alive == 1)
	{
	  if(enemyBullets[i].x > (ship -> x - ship -> bX) &&
	     enemyBullets[i].x < (ship -> x + ship -> bX) &&
	     enemyBullets[i].y > (ship -> y - ship -> bY) &&
	     enemyBullets[i].y < (ship -> y + ship -> bY))
	    {
	      ship -> lives -=1;
	      enemyBullets[i].alive = 0;
	    }
	}
    }
}

/*update autopilot x and y positions with respect to 
other enemies,bullets and presents coming thourh enterprise*/
void updateAutoEnterprise(enterpriseT *ship,enemyT enemies[],
			  int eSize,presentT *pre,int pSize,
			  bulletT *enemyBullets,int enSize)
{
  float angle,dist;
  float dX,dY;
  float x,y;
  enemyT * tmp = nearestEnemyToEnterprise(ship,enemies,eSize);
  presentT *tmpP = nearestPresent(ship,pre,pSize);
  bulletT *comingBull = comingBullet(ship,enemyBullets,enSize);

  /*find new enemy if older is dead*/
  if(((ship -> target -> alive == 0 ||
       (ship -> target -> x >= ship -> x + ship -> frameWidth)) &&
      tmp -> alive == 1))
    ship -> target = tmp;
  /*no alive enemy*/
  else if(ship -> target -> alive == 0 && tmp -> alive == 0)
    goToBase(ship);

  if(ship -> target -> alive == 1)
    {
      if(ship -> state == ENT_CHASE)
	{	 
	  /*bullet comes,runaway*/
	  if(ship -> target -> fired == 1 &&
	     ship -> target -> time >
	     ship -> target -> basedTime / 2 + 25 &&
	     (distance(0,ship -> y,
		       0,ship -> target -> y) <
	      (ship -> bY + 5)))
	    ship -> state = ENT_RUNAWAY;
	  else
	    {
	      /*bullet comes*/
	      if(comingBull -> alive == 1 &&
		 distance(ship -> x,ship -> y,
			  comingBull -> x,comingBull -> y) < 75)
		{
		  ship -> comingBullet = comingBull;
		  ship -> state = ENT_RUNFROMBULL;
		}
	      /*far away from base*/
	      else if(distance(ship -> x,ship -> y,
			       ship -> baseX,
			       ship -> baseY) > 500)
		ship -> state = ENT_STEPBACK;
	      /*present coming*/
	      else if(tmpP -> alive == 1 &&
		      distance(ship -> x,ship -> y,
			       tmpP -> x,tmpP -> y) < 200)
		{
		  ship -> state = ENT_CHASEPRE;
		  ship -> targetP = tmpP;
		}
	      /*chase*/
	      else
		{
		  dX = ship -> target -> x - ship -> x -
		    ship -> target -> frameWidth;
		  dY = ship -> target -> y - ship -> y;
		  dist = sqrt(dX * dX + dY * dY);
		  x = (dX / dist) * 5;
		  y = (dY / dist) * 5;
		  if(isThereAnotherObject(enemies,eSize,
					  ship -> x,
					  ship -> y + y) == 0)
		    ship -> y += y;
		  if(ship -> x >= 400)
		    ship -> x = 400;
		}
	    }
	}/*runaway from enemy*/
      else if(ship -> state == ENT_RUNAWAY)
	{
	  /* dY = ship -> target -> y - ship -> y; */
	  /* dX = ship -> target -> x - ship -> x; */
	  /* dist = sqrt(dX * dX + dY * dY); */
	  /* x = (dX / dist) * 4; */
	  /* y = (dY / dist) * 4; */

	  /* if(ship -> y > HEIGHT / 2 && */
	  /*    isThereAnotherObject(enemies,eSize, */
	  /* 			  ship -> x,ship -> y + y) == 0) */
	  /*   ship -> y += y; */
	  /* else if(ship -> y < HEIGHT / 2 && */
	  /* 	  isThereAnotherObject(enemies,eSize, */
	  /* 			       ship -> x,ship -> y - y) == 0) */
	  /*   ship -> y -= y;	     */

	  /* if(ship -> y <= 10) */
	  /*     ship -> y = 0; */
	  if(ship -> target -> time <
	     ship -> target -> basedTime / 2 + 25 &&
	     distance(0,ship -> y,
		      0,ship -> target -> y) > ship -> bY + 5)
	    ship -> state = ENT_CHASE;
	  else if(comingBull -> alive == 1 &&
		  distance(ship -> x,ship -> y,
			   comingBull -> x,comingBull -> y) < 75)
	    {
	      ship -> comingBullet = comingBull;
	      ship -> state = ENT_RUNFROMBULL;
	    }
	  else
	    {
	      angle = angleToTarget(ship -> x , ship -> y ,
				    ship -> target -> x,
				    ship -> target -> y);
	      /* ship -> x -= (2 * cos(angle)); */
	      ship -> y -= (2 * sin(angle));
	      if(ship -> y <= 5)
		{
		  ship -> y = 0;
		  ship -> x -= (2 * cos(angle));
		  if(ship -> x <= 35)
		    ship -> x = 35;
		}
	      else if(ship -> y + ship -> bY / 2 > HEIGHT)
		{
		  ship -> y = HEIGHT - ship -> bY / 2;
		  ship -> x -= (2 * cos(angle));
		  if(ship -> x <= 35)
		    ship -> x = 35;
		}
	    }
	}
      else if(ship -> state == ENT_STEPBACK)
	goToBase(ship);
      else if(ship -> state == ENT_IDLE)
	ship -> state = ENT_CHASE;
      /*chase the present*/
      else if(ship -> state == ENT_CHASEPRE)
	{
	  if(ship -> targetP -> alive == 0)
	    ship -> state = ENT_CHASE;
	  else
	    {
	      dX = ship -> targetP -> x - ship -> x;
	      dY = ship -> targetP -> y - ship -> y;
	      dist = sqrt(dX * dX + dY * dY);
	      y = (dY / dist) * 5;
	      x = (dX / dist) * 5;
	      if(isThereAnotherObject(enemies,eSize,
				      ship -> x + x,
				      ship -> y + y) == 0)
		ship -> y += y;
	    }
	}
      /*run away from bullet coming through enterprise*/
      else if(ship -> state == ENT_RUNFROMBULL)
	{
	  if(ship -> comingBullet -> alive == 0 ||
	     ship -> comingBullet -> x <= ship -> x)
	    ship -> state = ENT_CHASE;
	  else
	    {
	      dX = ship -> comingBullet -> x - ship -> x;
	      dY = ship -> comingBullet -> y - ship -> y;
	      dist = sqrt(dX * dX + dY * dY);
	      y = (dY / dist) * 3;
	      x = (dX / dist) * 3;
	      if(dY < ship -> bY && dY > 0)
		y = (ship -> bY / 6);
	      else if(dY > ship -> bY && dY < 0)
		y = -(ship -> bY / 6);		
	      if(isThereAnotherObject(enemies,eSize,
				      ship -> x - x,
				      ship -> y - y) == 0)
		  ship -> y -=(y);
	      
	      if(ship -> y <= 0)
		ship -> y = 0;
	      else if(ship -> y >= HEIGHT - ship -> bY / 2)
		ship -> y = HEIGHT - ship -> bY / 2;
	    }
	}

    }
}
/*return the nearest enemy to the enterprise*/
enemyT * nearestEnemyToEnterprise(enterpriseT *ship,
			       enemyT enemies[],int eSize)
{
  int i,x = -1;
  int minDist = 9999;
  enemyT *tmp = NULL;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  if(distance(ship -> x,ship -> y,enemies[i].x,enemies[i].y)
	     < minDist)
	    {
	      minDist = distance(ship -> x,ship -> y,
				 enemies[i].x,enemies[i].y);
	      x = i;
	    }
	}
    }
  if(x == -1)
    {
      tmp = malloc(sizeof(enemyT));
      tmp -> alive = 0;
    }
  else
    tmp = &(enemies[x]);  
  return tmp;
}
/*if no alive enemy then come back to base*/
 void goToBase(enterpriseT *ship)
 {
   float angle;
   if(distance(ship -> x , ship -> y ,
	       ship -> baseX , ship -> baseY) <= 10)
     {
       ship -> x = ship -> baseX;
       ship -> y = ship -> baseY;
       ship -> state = ENT_IDLE;
     }
   else
     {
       angle = angleToTarget(ship -> x , ship -> y ,
			     ship -> baseX,ship -> baseY);
       ship -> x += (2 * cos(angle));
       ship -> y += (2 * sin(angle));
       /*far enough from the enemy,then start chasing*/
       /* if(distance(ship -> x , ship -> y , nearest.x,nearest.y) */
       /* 	  > threshold * 2) */
       /* 	 ship -> state = ENT_CHASE; */
     }
 }
/*return the present type which is coming through enterprise*/
presentT *  nearestPresent(enterpriseT *ship,presentT *pre,int pSize)
{
  int i,x = -1;
  int minDist = 9999;
  presentT * tmp = NULL;
  for(i = 0 ; i< pSize ; ++i)
    {
      if(pre[i].alive == 1)
	{
	  if(distance(ship -> x,ship -> y,pre[i].x,pre[i].y) <
	     minDist)
	    {
	      minDist = distance(ship -> x,ship -> y,
				 pre[i].x,pre[i].y);
	      x = i;
	    }
	}
    }
  if(x == -1)
    {
      tmp = malloc(sizeof(presentT));
      tmp -> alive = 0;
    }
  else
    tmp = &(pre[x]);

  return tmp;
}
/*return the bullet type coming through the enterprise*/
bulletT * comingBullet(enterpriseT *ship,bulletT *enemyBullets,int bSize)
{
  int i,x = -1;
  int minDist = 999;
  bulletT *tmp = NULL;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(enemyBullets[i].alive == 1)
	{
	  if(distance(ship -> x,ship -> y,
		      enemyBullets[i].x,enemyBullets[i].y) < minDist)
	    {
	      minDist = distance(ship -> x,ship -> y,
				 enemyBullets[i].x,enemyBullets[i].y);
	      x = i;
	    }
	}
    }
  if(x == -1)
    {
      tmp =(bulletT *)malloc(sizeof(bulletT));
      tmp -> alive = 0;
    }
  else
    tmp = &(enemyBullets[x]);

  return tmp;
}
