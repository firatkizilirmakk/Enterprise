#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "../include/enemy.h"
#include "../include/main.h"
#include "../include/matfuncs.h"
#include "../include/explosion.h"

void initEnemy(enemyT enemies[] , int eSize,
	       ALLEGRO_BITMAP *images[])
{
  int i;
  for(i = 0; i < eSize ; ++i)
    {
      enemies[i].ID = ENEMY;
      enemies[i].alive = 0;
      enemies[i].state = ENMY_NORMAL;
      enemies[i].accelerated = 0;
      enemies[i].time = 0;
      enemies[i].basedTime = 85;
      enemies[i].baseX = WIDTH - 25;
      enemies[i].baseY = HEIGHT / 2;
      if(i < 5)
	{
	  enemies[i].type = STATIC;
	  enemies[i].speed = 1;
	  enemies[i].lives = 7;
	  enemies[i].loss = 7;
	  enemies[i].bX = 20;
	  enemies[i].bY = 19;
	  enemies[i].maxNumFrame = 10;
	  enemies[i].curNumFrame = 0;
	  enemies[i].frameNumCount = 0;
	  enemies[i].frameNumDelay = 10;
	  enemies[i].frameWidth = 36;
	  enemies[i].frameHeight = 29;
	  enemies[i].image = images[0];
	}
      else if(i >= 5 && i < 10)
	{
	  enemies[i].type = STATICF;
	  enemies[i].speed = 1;
	  enemies[i].lives = 14;
	  enemies[i].loss = 14;
	  enemies[i].bX = 25;
	  enemies[i].bY = 25;
	  enemies[i].maxNumFrame = 32;
	  enemies[i].curNumFrame = 0;
	  enemies[i].frameNumCount = 0;
	  enemies[i].frameNumDelay = 3;
	  enemies[i].frameWidth = 50;
	  enemies[i].frameHeight = 50;
	  enemies[i].image = images[1];
	}
      else if(i >= 10 && i < 15)
	{
	  enemies[i].type = STATICM;
	  enemies[i].basedTime = 150;
	  enemies[i].speed = 1;
	  enemies[i].lives = 14;
	  enemies[i].loss = 14;
	  enemies[i].bX = 25;
	  enemies[i].bY = 25;
	  enemies[i].maxNumFrame = 32;
	  enemies[i].curNumFrame = 0;
	  enemies[i].frameNumCount = 0;
	  enemies[i].frameNumDelay = 3;
	  enemies[i].frameWidth = 50;
	  enemies[i].frameHeight = 50;
	  enemies[i].image = images[2];
	}
      else if(i >= 15 && i < 20)
	{
	  enemies[i].type = DYNAMIC;
	  enemies[i].basedTime = 150;
	  enemies[i].speed = 2;
	  enemies[i].lives = 21;
	  enemies[i].loss = 21;
	  enemies[i].speedX = 2;
	  enemies[i].bX = 29;
	  enemies[i].bY = 34;
	  enemies[i].maxNumFrame = 2;
	  enemies[i].curNumFrame = 1;
	  enemies[i].frameNumCount = 0;
	  enemies[i].frameWidth = 75;
	  enemies[i].frameHeight = 70;
	  enemies[i].image = images[3];
	}
      else if(i >= 20 && i < 25)
	{
	  enemies[i].type = DYNAMICF;
	  enemies[i].state = ENMY_CHASE;
	  enemies[i].basedTime = 150;
	  enemies[i].speed = 2;
	  enemies[i].lives = 21;
	  enemies[i].loss = 21;
	  enemies[i].speedX = 2;
	  enemies[i].bX = 29;
	  enemies[i].bY = 34;
	  enemies[i].maxNumFrame = 2;
	  enemies[i].curNumFrame = 0;
	  enemies[i].frameNumCount = 0;
	  enemies[i].frameWidth = 75;
	  enemies[i].frameHeight = 70;
	  enemies[i].image = images[4];
	  enemies[i].coming = (bulletT *)malloc(sizeof(bulletT));
	}
    }
}
void drawEnemy(enemyT enemies[] , int eSize)
{
  int i;
  int sX,sY;/*for sourcex and sourcey when drawing a region*/
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  switch(enemies[i].type)
	    {	      
	    case STATIC:
	      sX = enemies[i].frameWidth * enemies[i].curNumFrame;
	      al_draw_bitmap_region(enemies[i].image,
				    sX,0,
				    enemies[i].frameWidth,
				    enemies[i].frameHeight,
				    enemies[i].x -
				    enemies[i].frameWidth / 2,
				    enemies[i].y -
				    enemies[i].frameHeight / 2,
				    0);
	      break;
	    case STATICF:
	    case STATICM:
	      sY = enemies[i].curNumFrame * enemies[i].frameHeight;
	      al_draw_bitmap_region(enemies[i].image,
				    0,sY,
				    enemies[i].frameWidth,
				    enemies[i].frameHeight,
				    enemies[i].x -
				    enemies[i].frameWidth / 2,
				    enemies[i].y -
				    enemies[i].frameWidth / 2,0);     	      break;
	    case DYNAMIC:
	    case DYNAMICF:
	      sX = enemies[i].curNumFrame * enemies[i].frameWidth;
	      al_draw_bitmap_region(enemies[i].image,
				    sX,0,
				    enemies[i].frameWidth,
				    enemies[i].frameHeight,
				    enemies[i].x -
				    enemies[i].frameWidth / 2,
				    enemies[i].y -
				    enemies[i].frameHeight / 2,
				    0);
	      break;
	    }
	}
    }
  
}
/*start the enemies with respect to difficulty*/
void startEnemy(enemyT enemies[] , int eSize ,
		gameT *gT)
{
  int i = 0,k = 0;
  int x,y;
  int prob = 1500 / (gT -> difficulty);
  /*if at least one alive enemy no need to decrease the probability*/
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	break;
    }

  if(i == eSize)
    prob = 50;

  /*arrange probability and number of enemies*/
  if(gT -> difficulty == 1)
    {
      prob = 50;
      k = 5;
      i = 0;
    }
  else if(gT -> difficulty < 5)
    {
      i = gT -> difficulty + 1;
      k = i + 6 + gT -> difficulty / 4;
    }
  else if(gT -> difficulty < 8)
    {
      i = gT -> difficulty + 3;
      k = i + 7 + gT -> difficulty / 7;
    }
  else
    {
      i = gT -> difficulty + 5;
      k = i + 7;
      if(gT -> difficulty == 10)
  	k = i + 6;
    }
  /*start the enemy with random y position */
  for(; i < k ; ++i)
    {
      if(enemies[i].alive == 0)
	{
	  if(rand() % prob == 0)
	    {
	      enemies[i].alive = 1;
	      enemies[i].time = 0;
	      enemies[i].fired = 0;
	      enemies[i].loss = enemies[i].lives;
	      do{
		x = (WIDTH) - 20;
		y = 30 + rand() % (HEIGHT - 80); 
	      }while(isThereAnotherObject(enemies,eSize,x,y));
	      enemies[i].bornX = x;
	      enemies[i].bornY = y;
	      enemies[i].x = x;
	      enemies[i].y = y;
	      enemies[i].baseX = WIDTH - 25;
	      enemies[i].baseY = HEIGHT / 2;
	      enemies[i].tarX = -1;
	      enemies[i].tarY = -1;
	      if(enemies[i].type == DYNAMICF)
		{
		  enemies[i].state = ENMY_CHASE;
		  enemies[i].coming -> alive = 0;
		}
	      break;
	    }
	}
    }
}
/*update enemies' frames and positions*/
void updateEnemy(enemyT enemies[] , int eSize ,
		 enterpriseT *ship,
		 bulletT *bullets,
		 int bSize)
{
  int i;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  /*passed based time not allowed to shoot now*/
	  enemies[i].time +=1;
	  if(enemies[i].time >= enemies[i].basedTime)
	    enemies[i].fired = 0;
	  
	  /*arrange enemies' frame things and x,y positions*/
	  switch(enemies[i].type)
	    {/*fix values*/
	    case STATIC:
	      enemies[i].frameNumCount +=1;
	      if(enemies[i].frameNumCount >=
		 enemies[i].frameNumDelay)
		{
		  enemies[i].curNumFrame +=1;
		  if(enemies[i].curNumFrame >=
		     enemies[i].maxNumFrame)
		    enemies[i].curNumFrame = 0;
		  enemies[i].frameNumCount = 0;
		}
	      enemies[i].x -= enemies[i].speed;
	      break;
	    case STATICF:
	    case STATICM:
	      enemies[i].frameNumCount +=1;
	      if(enemies[i].frameNumCount >=
		 enemies[i].frameNumDelay)
		{
		  enemies[i].curNumFrame +=1;
		  if(enemies[i].curNumFrame >=
		     enemies[i].maxNumFrame)
		    enemies[i].curNumFrame = 0;
		  enemies[i].frameNumCount = 0;
		}
	      enemies[i].x -= enemies[i].speed;
	      break;/*can not move*/
	    case DYNAMIC:
	      enemies[i].x -= enemies[i].speed;
	      break;
	    case DYNAMICF:
	      autoEnemy(ship,&enemies[i],bullets,bSize);
	      break;
	    }

	  if(enemies[i].x <= 0)
	    enemies[i].alive = 0;
	}
    }
}
/*return 0 if no enemy on immediate environment 
 * 0 otherwise
 */
int isThereAnotherObject(enemyT enemies[],int eSize,int x,int y)
{
  int retVal = 0;
  int i;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  if(x < (enemies[i].x + enemies[i].frameWidth + 25) &&
	     x > (enemies[i].x - enemies[i].frameWidth - 25) &&
	     y < (enemies[i].y + enemies[i].frameHeight + 25) &&
	     y > (enemies[i].y - enemies[i].frameHeight - 25))
	    {
	      retVal = 1;
	      break;
	    }
	}
    }
  return retVal;
}

void initEnemyBullet(bulletT bullets[],int bSize)
{
  int i;
  for(i = 0 ; i < bSize ; ++i)
    {
      bullets[i].ID = ENEMY_BULL;
      bullets[i].alive = 0;
      bullets[i].diffX = -1;
      bullets[i].diffY = -1;
      bullets[i].routed = 0;
      bullets[i].speed = 7;
      bullets[i].vX = 2 + rand() % 2;
      bullets[i].vX = 2 + rand() % 2;
    }
}

/*fire the enemy bullet with respect to it's based time,type and 
fired situation*/
void fireEnemyBullet(bulletT bullets[],int bSize,
		     enemyT enemies[],int eSize)
{
  int i,j;
  for(j = 0 ; j < eSize ; ++j)
    {
      if(enemies[j].alive == 1 && enemies[j].type != STATIC &&
	 enemies[j].time >= enemies[j].basedTime &&
	 enemies[j].fired == 0)
	{
	  for(i = 0 ; i < bSize ; ++i)
	    {
	      if(bullets[i].alive == 0)
		{
		  bullets[i].x = enemies[j].x - enemies[j].bX;
		  bullets[i].y = enemies[j].y;
		  bullets[i].type = enemies[j].type;
		  bullets[i].alive = 1;
		  bullets[i].routed = 1;
		  bullets[i].diffX = -1;
		  bullets[i].diffY = -1;
		  enemies[j].time = 0;
		  enemies[j].fired = 1;
		  bullets[i].vX = 2 + rand() % 2;/*no need anymore*/
		  bullets[i].vX = 2 + rand() % 2;
		  break;
		}
	    }
	}
    }
}

/*update enemy bullet's x and y positions with respect to
it's type*/
void updateEnemyBullet(bulletT bullets[],int bSize,enterpriseT *ship)
{
  int i;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(bullets[i].alive == 1)
	{
	  if(bullets[i].type == STATICM)
	    {
	      /*bullet is routed no need to calculate differences*/
	      if(bullets[i].routed == 1)
		{
		  bullets[i].diffX = bullets[i].x - ship -> x;
		  bullets[i].diffY = bullets[i].y - ship -> y;
		  bullets[i].routed = 0;		    
		}
	      if(bullets[i].diffX != -1 && bullets[i].diffY != -1 &&
		 bullets[i].diffX > ship -> bX)
		{
		  bullets[i].y -= (bullets[i].diffY /
				   (bullets[i].diffX /
				    (bullets[i].speed + rand() % 4)));
		}
	    }
	      
	      /* if(bullets[i].diffX != -1 && bullets[i].diffY != -1 && */
	      /* 	 ship -> x - ship -> frameWidth <= bullets[i].x) */
	      /* 	{ */
	      /* 	  dX = bullets[i].diffX - bullets[i].x; */
	      /* 	  dY = bullets[i].diffY - bullets[i].y; */
	      /* 	  dist = sqrt(dX * dX + dY * dY); */
	      /* 	  x = (dX / dist) * 5; */
	      /* 	  y = (dY / dist) * 5; */
	      /* 	  bullets[i].vX += x; */
	      /* 	  bullets[i].vY += y; */
	      /* 	  bullets[i].x += bullets[i].vX / 10; */
	      /* 	  bullets[i].y += bullets[i].vY / 10; */
	      /* 	} */
	      /* else if(ship -> x - ship -> frameWidth > bullets[i].x) */
	      /* 	{ */
	      /* 	  dX = 0 - bullets[i].x; */
	      /* 	  dY = HEIGHT / 2 - bullets[i].y; */
	      /* 	  dist = sqrt(dX * dX + dY * dY); */
	      /* 	  x = (dX / dist) * 5; */
	      /* 	  y = (dY / dist) * 5; */
	      /* 	  bullets[i].x += x; */
	      /* 	  bullets[i].y += y; */
	      /* 	} */
	    bullets[i].x -= bullets[i].speed;
	  if(bullets[i].x <= 0 ||
	     bullets[i].y <= 0 ||
	     bullets[i].y >= HEIGHT)
	    {
	      bullets[i].alive = 0;
	    }
	}
    }
}

void drawEnemyBullet(bulletT bullets[],int bSize)
{
  int i;
  ALLEGRO_COLOR blue = al_map_rgb(0,125,255);
  ALLEGRO_COLOR red = al_map_rgb(255,255,255);
  ALLEGRO_COLOR color;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(bullets[i].alive == 1)
	{
	  color = blue;
	  if(bullets[i].type == STATICM)
	    color = red;
	  al_draw_circle(bullets[i].x,bullets[i].y,4,color,1);
	}
    }
}
/*when the intelligent enemy comes,arrange it's position with
respect to enterprise and bullets coming through it*/
void autoEnemy(enterpriseT *ship,enemyT *enemy,
	       bulletT *bullets,int bSize)
{
  int threshold = 200;
  float dx,dy,x,y,dist;
  bulletT *coming = bulletToEnemy(enemy,bullets,bSize);
  /*find the intelligent enemy*/
  if(enemy -> state == ENMY_CHASE)
    {
      if(distance(enemy -> x,enemy -> y,enemy -> baseX,enemy -> baseY) > threshold  * 2)
	enemy -> state = ENMY_STEPBACK;
      else
	{
	  dx = ship -> x - enemy -> x;
	  dy = ship -> y - enemy -> y;
	  dist = sqrt(dx * dx + dy * dy);
	  x = (dx / dist) * 3;
	  y = (dy / dist) * 3;
	  enemy -> x += x;
	  enemy -> y += y;
	  /* angle = angleToTarget(enemy -> x , enemy -> y, */
	  /* 			ship -> x,ship -> y); */
	  /* /\* enemy -> x += (2 * cos(angle)); *\/ */
	  /* enemy -> y += (2 * sin(angle)); */
	  /* if(enemy -> x >= WIDTH) */
	  /*   enemy -> x = WIDTH; */
	  if(enemy -> x <= 2 * WIDTH / 3)
	    enemy -> x = 2 * WIDTH / 3;
	  if(coming -> alive == 1 &&
	     distance(enemy -> x,enemy -> y,
		      coming -> x,coming ->y) < 150)
	    {
	      enemy -> state = ENMY_RUNAWAY;
	      enemy -> coming = coming;
	    }
	}
    }
  else if(enemy -> state == ENMY_STEPBACK)
    {
      if(distance(enemy -> x,enemy -> y,
		  enemy -> baseX,enemy -> baseY) <= 10)
	{
	  enemy -> x = enemy -> baseX;
	  enemy -> y = enemy -> baseY;
	  enemy -> state = ENMY_CHASE;
	}
      else
	{
	  dx = enemy -> baseX - enemy -> x;
	  dy = enemy -> baseY - enemy -> y;
	  dist = sqrt(dx * dx + dy * dy);
	  x = dx / dist * 3;
	  y = dy / dist * 3;
	  enemy -> x += x;
	  enemy -> y += y;
	}
    }
  else if(enemy -> state == ENMY_RUNAWAY)
    {
      if(enemy -> coming -> alive == 0 ||
	 enemy -> coming -> x >
	 enemy -> x)
	enemy -> state = ENMY_CHASE;
      else
	{
	  dx = enemy -> coming -> x - enemy -> x;
	  dy = enemy -> coming -> y - enemy -> y;
	  dist = sqrt(dx * dx + dy * dy);
	  y = (dy / dist) * 3;
	  enemy -> y -=y;
	}
    }
}
/*return the nearest bullet coming to enemy*/
bulletT * bulletToEnemy(enemyT *enemy,bulletT *bullets,int bSize)
{
  int i,x = -1;
  int minDist = 999;
  bulletT *tmp = NULL;
  for(i = 0 ; i < bSize ; ++i)
    {
      if(bullets[i].alive == 1 &&
	 distance(enemy -> x,enemy -> y ,bullets[i].x,bullets[i].y)
	 < minDist)
	{
	  minDist = distance(enemy -> x,enemy -> y ,
			     bullets[i].x,bullets[i].y);
	  x = i;
	}
    }
  if(x == -1)
    {
      tmp = malloc(sizeof(bulletT));
      tmp -> alive = 0;
    }
  else
    tmp = &(bullets[x]);

  return tmp;
}
/*if enemy collides with the eterprise*/
void enterpriseCollision(enterpriseT *ship,enemyT *enemies,int eSize
			 ,explosionT *explosions,int exSize)
{
  int i;
  for(i = 0 ; i < eSize ; ++i)
    {
      if(enemies[i].alive == 1)
	{
	  if((ship -> x + ship -> bX / 2) >
	     (enemies[i].x - enemies[i].bX / 2) &&
	     (ship -> x - ship -> bX / 2) <
	     (enemies[i].x + enemies[i].bX / 2) &&
	     (ship -> y + ship -> bY / 2) >
	     (enemies[i].y - enemies[i].bY / 2) &&
	     (ship -> y - ship -> bY / 2) <
	     (enemies[i].y + enemies[i].bY / 2))
	    {
	      enemies[i].alive = 0;
	      ship -> lives -=1;
	      startExplosion(explosions,exSize,
			     enemies[i].x,enemies[i].y);
	    }
	}
    }
}
