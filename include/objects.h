#ifndef OBJECTS_H
#define OBJECTS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
/*id's of objects*/
typedef enum {ENTERPRISE,ENEMY,BULLET,ENEMY_BULL,PRESENT}ids;

typedef struct enemyT enemyT;

typedef struct bulletT
{
  int ID,
    type,
    x,y,
    speed,
    count,/*up,down or straigt bullet,for laser bullet*/
    damage,/*damage it can give*/
    diffX,/*differences between bullet and enemy*/
    diffY,
    bX,bY,/*bounding boxes*/
    vX,vY,/*for missiles and lightenings*/
    routed,/*whether it is routed to nearest enemy*/
    enemySpeed,
    time,
    alive;/*whether alive*/

  enemyT *enemy;
  
  int curNumFrame,
    maxNumFrame,
    frameNumCount,
    frameNumDelay,
    frameWidth,
    frameHeight,
    spriteRows,
    spriteColumns;
  
  ALLEGRO_BITMAP *image;
  ALLEGRO_SAMPLE *sound;
}bulletT;

struct enemyT
{
  /*object variables*/
  int ID,
    type,
    x,y,
    bornX,
    bornY,/*born positions*/
    tarX,
    tarY,/*target x and y values for dynamic enemy*/
    speed,
    speedX,/*value of acceleration*/
    accelerated,/*whether it is accelerated*/
    bX,bY,/*bounding for x and y positions*/
    alive,/*whether alive*/
    lives,/*lives enemy has*/
    loss,/*lost lives*/
    baseX,baseY,
    time,
    basedTime,/*time shooting based*/
    fired,
    state;/*states for intelligent enemy*/

  bulletT *coming;

  /*object's bitmap variables*/
  int curNumFrame,
    maxNumFrame,
    frameNumCount,
    frameNumDelay,
    frameWidth,
    frameHeight,
    spriteRows,
    spriteColumns;

  ALLEGRO_BITMAP *image;
};

typedef struct
{
  int ID,
    type,
    x,y,
    speed,
    bX,bY,
    alive,
    time,
    caughtByEnt;/*if present is caught*/

  int frameWidth,
    frameHeight;
  ALLEGRO_BITMAP *image;
  ALLEGRO_SAMPLE *sound;
}presentT;

typedef struct
{
  /*object variables*/
  int ID,
    type,
    x,y,
    bX,bY,/*bounding for x and y positions*/
    score,
    speed,
    lives,
    state,/*in which states ship*/
    baseX,baseY,/*for autopilot base*/
    time,/*time for continious shooting*/
    presentTime,/*started time after present caught*/
    caughtPresent;
  
  enemyT *target;
  presentT *targetP;
  bulletT *comingBullet;
  
  /*object's bitmap variables*/
  int curNumFrame,
    maxNumFrame,
    frameNumCount,
    frameNumDelay,
    frameWidth,
    frameHeight,
    spriteRows,
    spriteColumns;

  ALLEGRO_BITMAP *image;
}enterpriseT;



typedef struct
{
  int x,y,alive;

  /*object's bitmap variables*/
  int curNumFrame,
    maxNumFrame,
    frameNumCount,
    frameNumDelay,
    frameWidth,
    frameHeight;
  ALLEGRO_BITMAP *image;
  ALLEGRO_SAMPLE *sound;
}explosionT;


typedef struct
{
  /*background positions,speed,resolution,image*/
  float x,y;
  float vX,vY;
  int dirX,dirY;
  int width,height;
  ALLEGRO_BITMAP * image;
}backgroundT;

typedef struct
{
  int c;/*stores which word is chosen*/
  int words;/*how many words*/
  char strings[3][11];
  int pressed;/*whether key is pressed*/
  ALLEGRO_FONT *font;
}menuT;

typedef struct
{
  char name[25];
  int score;
}highScoreT;

/*when present is caught change effect of enterprise*/
typedef struct 
{
  int x,y,alive,time;

  int curNumFrame,
    maxNumFrame,
    frameNumCount,
    frameNumDelay,
    frameWidth,
    frameHeight;
  ALLEGRO_BITMAP *image;
  
}changeT;

#endif

