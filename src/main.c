#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/main.h"
#include "../include/objects.h"
#include "../include/enterprise.h"
#include "../include/bullet.h"
#include "../include/enemy.h"
#include "../include/spawn.h"
#include "../include/background.h"
#include "../include/menu.h"
#include "../include/explosion.h"
#include "../include/highscore.h"
#include "../include/change.h"

int main(int argc, char *argv[])
{
  /*game variables*/
  int redraw = 0;/*to draw when it is time for timer event*/
  int keys[9] = {0,0,0,0,0,0,0,0};/*to understand when keys pressed*/
  int gameState = MENU;/*initial state*/
  int pos;/*for input*/
  int i;
  char names[25];
  char fileScore [] = "../highscores.bin";
  int showS = 0;
  
  /*object variables*/
  enterpriseT ship;
  bulletT *bullets,*enemyBullets;
  enemyT *enemies;
  backgroundT bg,fg,mg;
  menuT menu;
  gameT gT;
  presentT pre[3];/*three fixed presents*/
  explosionT *explosion;
  highScoreT scores[11];
  changeT change;
  
  /*allegro variables*/
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_DISPLAY_MODE dispMode;
  ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
  ALLEGRO_TIMER *timer = NULL;
  ALLEGRO_FONT *menuFont = NULL;
  ALLEGRO_FONT *statisticFont = NULL;
  ALLEGRO_FONT *titleFont = NULL;
  
  /*object bitmaps*/
  ALLEGRO_BITMAP *enterpriseImage;
  ALLEGRO_BITMAP *enemyImages[5];
  ALLEGRO_BITMAP *missilePreImage,
    *laserPreImage,
    *lighteniningPreImage;
  ALLEGRO_BITMAP *bulletImages[3];
  ALLEGRO_BITMAP *background,*foreground,*middleground;
  ALLEGRO_BITMAP *explosionImage;
  ALLEGRO_BITMAP *autoPilotImage;
  ALLEGRO_BITMAP *livesImage;
  ALLEGRO_BITMAP *changeImage;
  
  /*sounds*/
  ALLEGRO_SAMPLE *shootSamples[4];
  ALLEGRO_SAMPLE *explosionSample = NULL;
  ALLEGRO_SAMPLE *enterpriseFall = NULL;
  ALLEGRO_SAMPLE *presentTaken = NULL;
  
  /*string for user input*/
  ALLEGRO_USTR *str = NULL;
  
  /*initiliaze allegro*/
  if(!al_init())
    return -1;

  display = al_create_display(WIDTH,HEIGHT);
  al_set_window_position(display,175,75);
  al_set_window_title(display,"Enterprise");

  /*initialize allegro things*/
  al_install_keyboard();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(4);


  srand(time(NULL));
  /*initialize game object and create object arrays*/
  initGameT(&gT);
  enemies = (enemyT *)malloc(sizeof(enemyT) * (gT.numEnemies));
  bullets = (bulletT *)malloc(sizeof(bulletT) * (gT.numBullets));
  enemyBullets = (bulletT *)malloc(sizeof(bulletT) *
				   (gT.numEnemyBullets));
  explosion = (explosionT *)malloc(sizeof(explosionT) *
				   (gT.numExplosions));
  /*load bitmaps*/
  enterpriseImage = al_load_bitmap("../media/enterprise.png");
  enemyImages[0] = al_load_bitmap("../media/astroid.PNG");
  enemyImages[1] = al_load_bitmap("../media/static.PNG");
  enemyImages[2] = al_load_bitmap("../media/staticm.png");
  enemyImages[3] = al_load_bitmap("../media/dynamic.PNG");
  enemyImages[4] = al_load_bitmap("../media/dynamic.PNG");

  
  laserPreImage = al_load_bitmap("../media/laser.bmp");
  missilePreImage = al_load_bitmap("../media/missile.bmp");
  lighteniningPreImage = al_load_bitmap("../media/lightening.bmp");

  bulletImages[1] = al_load_bitmap("../media/laser.png");
  bulletImages[0] = al_load_bitmap("../media/missile.png");
  bulletImages[2] = al_load_bitmap("../media/lightening.png");

  background = al_load_bitmap("../media/bg.PNG");
  foreground = al_load_bitmap("../media/fg.PNG");
  middleground = al_load_bitmap("../media/stars.PNG");

  autoPilotImage = al_load_bitmap("../media/autopilot.PNG");
  livesImage = al_load_bitmap("../media/heart.PNG");
  
  explosionImage = al_load_bitmap("../media/explosion.png");
  changeImage = al_load_bitmap("../media/change.PNG");
  
  al_convert_mask_to_alpha(enemyImages[1],al_map_rgb(255,0,255));
  al_convert_mask_to_alpha(enemyImages[2],al_map_rgb(255,0,255));
  al_convert_mask_to_alpha(explosionImage,al_map_rgb(255,0,255));
  al_convert_mask_to_alpha(changeImage,al_map_rgb(255,255,255));
  /*load sounds*/
  shootSamples[0] = al_load_sample("../media/shoot.WAV");
  shootSamples[1] = al_load_sample("../media/missile.wav");
  shootSamples[2] = al_load_sample("../media/laser.wav");
  shootSamples[3] = al_load_sample("../media/lightening.wav");
  explosionSample = al_load_sample("../media/explode.wav");
  presentTaken = al_load_sample("../media/takenpresent.wav");
  enterpriseFall = al_load_sample("../media/enterprisefall.wav");
  
  /*load fonts*/
  menuFont = al_load_font("../media/menu.ttf",24,0);
  statisticFont = al_load_font("../media/statistic.ttf",18,0);
  titleFont = al_load_font("../media/title.ttf",72,0);
  
  /*initialize objects*/
  initEnterprise(&ship,enterpriseImage,&dispMode,MISSILE);
  initEnterpriseBullet(bullets,gT.numBullets,
		       bulletImages,shootSamples);
  initEnemy(enemies,gT.numEnemies,enemyImages);
  initEnemyBullet(enemyBullets,gT.numEnemyBullets);
  initPresent(&pre[0],laserPreImage,LASER,presentTaken);
  initPresent(&pre[1],missilePreImage,MISSILE,presentTaken);
  initPresent(&pre[2],lighteniningPreImage,LIGHTENING,presentTaken);
  initBackground(&bg,0,0,3,0,1366,768,-1,1,background);
  initBackground(&fg,0,0,10,0,1366,768,-1,1,foreground);
  initBackground(&mg,-200,0,3,0,1600,600,-1,1,middleground);
  initMenu(&menu,menuFont);
  initExplosion(explosion,gT.numExplosions,
		explosionImage,explosionSample);
  initChange(&change,changeImage);
  
  /*create event queue,cycle 60 times per second*/
  eventQueue = al_create_event_queue();
  timer = al_create_timer(1.0 / FPS );

  /*add events to the queue*/
  al_register_event_source(eventQueue,al_get_keyboard_event_source());
  al_register_event_source(eventQueue,
			   al_get_timer_event_source(timer));
  al_register_event_source(eventQueue,
			   al_get_display_event_source(display));
  /*string for user input*/
  str = al_ustr_new("");
  pos = (int)al_ustr_size(str);
  al_start_timer(timer);
  while(gameState != EXIT)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(eventQueue , &event);
      if(event.type == ALLEGRO_EVENT_TIMER)
	{
	  redraw = 1;
	  updateBackground(&fg);
	  /*paused the game*/
	  if(gameState == PLAY && keys[ESC] == 1)
	    gameState = PAUSE;
	  else if(gameState == PAUSE)
	    {
	      if(keys[ESC] == 0)
		gameState = PLAY;
	      else if(keys[QUIT] == 1)
		gameState = EXIT;
	    }
	  /*wants to play again,initialize every thing*/
	  else if(gameState == OVER && keys[SPACE] == 1 && showS == 1)
	    {
	      gameState = PLAY;
	      initGameT(&gT);
	      initEnterprise(&ship,enterpriseImage,&dispMode,MISSILE);
	      initEnterpriseBullet(bullets,gT.numBullets,
				   bulletImages,shootSamples);
	      initEnemy(enemies,gT.numEnemies,enemyImages);
	      initEnemyBullet(enemyBullets,gT.numEnemyBullets);/*?*/
	      initPresent(&pre[0],laserPreImage,LASER,presentTaken);
	      initPresent(&pre[1],missilePreImage,
			  MISSILE,presentTaken);
	      initPresent(&pre[2],lighteniningPreImage,
			  LIGHTENING,presentTaken);
	      initBackground(&bg,0,0,1,0,1366,768,-1,1,background);
	      initBackground(&fg,0,0,6,0,1366,768,-1,1,foreground);
	      initMenu(&menu,menuFont);
	      initExplosion(explosion,gT.numExplosions,
			    explosionImage,explosionSample);
	      initChange(&change,changeImage);
	      
	      str = al_ustr_new("");
	      pos = (int)al_ustr_size(str);
	      showS = 0;
	      /*init keys also*/
	      for(i = 0 ; i < 9 ; ++i)
		keys[i] = 0;
	    }
	  /*no again*/
	  else if(gameState == OVER && keys[QUIT] == 1 && showS == 1)
	    gameState = EXIT;
	  /*playing the game*/
	  if(gameState == PLAY)
	    {
	      updateBackground(&bg);
	      updateBackground(&mg);
	      drawStatistic(ship.score,ship.lives,ship.state,
			    livesImage,autoPilotImage,statisticFont);
	      callMovement(keys,&ship,&dispMode);
	      startEnemy(enemies,gT.numEnemies,&gT);
	      updateEnemy(enemies,gT.numEnemies,&ship,
			  bullets,gT.numBullets);
	      gT.time +=1;
	      if(gT.time % 50 == 0)
		fireEnterpriseBullet(bullets,gT.numBullets,
				     &ship,ship.type);
	      if(gT.time >= 850)
		{
		  changeDifficulty(&gT);
		  if(gT.difficulty != gT.topDiff)
		    ship.lives +=5;
		}
	      /*mode switched to auto pilot*/
	      if(ship.state != NORMAL)
		updateAutoEnterprise(&ship,enemies,
				     gT.numEnemies,pre,3,
				     enemyBullets,gT.numEnemyBullets);

	      fireEnemyBullet(enemyBullets,gT.numEnemyBullets,
			      enemies,gT.numEnemies);
	      updateEnterpriseBullet(bullets,gT.numBullets,
				     enemies,gT.numEnemies,
				     &dispMode);
	      updateEnemyBullet(enemyBullets,gT.numEnemyBullets,
				&ship);
	      bulletCollision(bullets,gT.numBullets,
			      enemies,gT.numEnemies,
			      explosion,gT.numExplosions,
			      &(ship.score));
	      collideEnterprise(&ship,enemyBullets,
				gT.numEnemyBullets);
	      enterpriseCollision(&ship,enemies,gT.numEnemies,
				  explosion,gT.numExplosions);
	      updateExplosion(explosion,gT.numExplosions);
	      startPresent(pre,3,&gT,&dispMode);
	      updatePresent(pre,3,&ship,gT.difficulty);
	      preCollision(pre,3,&ship,&change);
	      updateDrawChange(&change,&ship);
	      if(ship.lives <= 0)
		{
		  gameState = OVER;
		  al_play_sample(enterpriseFall,0.75,0,1,
				 ALLEGRO_PLAYMODE_ONCE,
				 NULL);
		}
	    }/*end of play state*/
	  else if(gameState == MENU)
	    arrangeMenu(&menu,keys,&gameState);
	  /*read highscores*/
	  else if(gameState == HIGHSCORE)
	    {
	      syncScores(fileScore,scores,"empty",0);
	      if(keys[ESC] == 1)
		{
		  gameState = MENU;
		  keys[ESC] = 0;
		}
	    }

	}
      else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
	switchEventType(&event,keys,0,&(menu.pressed));
      else if(event.type == ALLEGRO_EVENT_KEY_UP)
	switchEventType(&event,keys,1,&(menu.pressed));
      else if(event.type == ALLEGRO_EVENT_KEY_CHAR &&
	      gameState == OVER && showS == 0)
	{
	  /*get user name when game over*/
	  if(event.keyboard.unichar >= 32)
	    pos +=al_ustr_append_chr(str,
				     event.keyboard.unichar);
	  else if(event.keyboard.keycode ==
		  ALLEGRO_KEY_BACKSPACE)
	    {
	      if(al_ustr_prev(str,&pos))
		al_ustr_truncate(str,pos);
	    }
	  else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
	    {
	      al_ustr_to_buffer(str,names,25);
	      syncScores(fileScore,scores,names,ship.score);
	      showS = 1;
	    }
	}

      /*draw objects*/
      if(redraw == 1 && al_is_event_queue_empty(eventQueue))
      	{
      	  redraw = 0;
	  drawBackground(&fg,&dispMode);
	  if(gameState == PAUSE)
	    {
	      al_draw_text(menuFont,al_map_rgb(0,177,255),
			   WIDTH/ 2 - 85,
			   200,
			   0,
			   "Paused");
	      al_draw_textf(menuFont,al_map_rgb(0,177,255),
			    150, 10,
			    ALLEGRO_ALIGN_CENTER,
			    "Press Q to quit");
	    }
	  else if(gameState == PLAY)
	    {
	      drawBackground(&bg,&dispMode);
	      drawBackground(&mg,&dispMode);
	      drawEnterprise(&ship);
	      drawEnterpriseBullet(bullets,gT.numBullets);
	      drawEnemyBullet(enemyBullets,gT.numEnemyBullets);
	      drawPresent(pre,3);
	      drawEnemy(enemies,gT.numEnemies);
	      drawExplosion(explosion,gT.numExplosions);
	    }
	  else if(gameState == MENU)
	    {
	      al_draw_text(titleFont,al_map_rgb(0,177,250),
			   WIDTH / 2 - 135,
			   130,0,"Enterprise");
	      drawMenu(&menu,&dispMode);
	    }
	  else if(gameState == OVER)
	    {
	      al_draw_textf(menuFont,al_map_rgb(255,255,255),
			    WIDTH / 2 - 30, 60,
			    ALLEGRO_ALIGN_CENTER,"You lost !");

	      al_draw_textf(menuFont,al_map_rgb(0,177,255),
			    WIDTH / 2 - 30, 90,
			    ALLEGRO_ALIGN_CENTER,"Your score : %d",
			    ship.score);
	      al_draw_textf(menuFont,al_map_rgb(255,255,255),
			    WIDTH / 2 - 30, 120,
			    ALLEGRO_ALIGN_CENTER,"Enter your name");
	      al_draw_textf(menuFont,al_map_rgb(255,255,255),
			    230, 10,
			    ALLEGRO_ALIGN_CENTER,"Press space to play again");
	      al_draw_textf(menuFont,al_map_rgb(255,255,255),
			    150, 50,
			    ALLEGRO_ALIGN_CENTER,
			    "Press Q to quit");
	      al_draw_ustr(menuFont, al_map_rgb(255,255,255),
			   WIDTH / 2 - 20, 150,
			   ALLEGRO_ALIGN_CENTRE, str);
	      if(showS == 1)
		showScores(scores,menuFont);
	    }
	  else if(gameState == HIGHSCORE)
	    showScores(scores,menuFont);
	  al_flip_display();
	  al_clear_to_color(al_map_rgb(0 , 0 , 0));
	}
    }
  /*destroy object bitmaps,fonts,timer,display*/
  al_destroy_sample(shootSamples[0]);
  al_destroy_sample(shootSamples[1]);
  al_destroy_sample(shootSamples[2]);
  al_destroy_sample(shootSamples[3]);
  al_destroy_font(menuFont);
  al_destroy_bitmap(enterpriseImage);
  al_destroy_bitmap(enemyImages[4]);
  al_destroy_bitmap(enemyImages[3]);
  al_destroy_bitmap(enemyImages[2]);
  al_destroy_bitmap(enemyImages[1]);
  al_destroy_bitmap(enemyImages[0]);
  al_destroy_bitmap(missilePreImage);
  al_destroy_bitmap(laserPreImage);
  al_destroy_bitmap(lighteniningPreImage);
  al_destroy_bitmap(bulletImages[0]);
  al_destroy_bitmap(bulletImages[1]);
  al_destroy_bitmap(bulletImages[2]);
  al_destroy_event_queue(eventQueue);
  al_destroy_timer(timer);
  al_destroy_display(display);  
  return 0;
}

/*if a key is pressed switch its value from 0 to 1 and viceversa*/
void switchEventType(ALLEGRO_EVENT *event ,int keys[] ,
		     int type ,int *keyPressed)
{
  /*when a key is pressed*/
  if(type == 0)
    {
      switch(event -> keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	  keys[UP] = 1;
	  break;
	case ALLEGRO_KEY_DOWN:
	  keys[DOWN] = 1;
	  break;
	case ALLEGRO_KEY_LEFT:
	  keys[LEFT] = 1;
	  break;
	case ALLEGRO_KEY_RIGHT:
	  keys[RIGHT] = 1;
	  break;
	case ALLEGRO_KEY_SPACE:
	  if(keys[SPACE] == 1)
	    keys[SPACE] = 0;
	  else
	    keys[SPACE] = 1;
	  break;
	case ALLEGRO_KEY_ENTER:
	  keys[ENTER] = 1;
	  break;
	case ALLEGRO_KEY_A:
	  if(keys[AUTO] == 1)
	    keys[AUTO] = 0;
	  else
	    keys[AUTO] = 1;
	  break;
	case ALLEGRO_KEY_ESCAPE:
	  if(keys[ESC] == 1)
	    keys[ESC] = 0;
	  else
	    keys[ESC] = 1;
	  break;
	case ALLEGRO_KEY_Q:
	    keys[QUIT] = 1;
	  break;
	}
    }
  /*the key not pressed anymore*/
  else if(type == 1)
    {
      switch(event -> keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
	  keys[UP] = 0;
	  *keyPressed = 0;
	  break;
	case ALLEGRO_KEY_DOWN:
	  keys[DOWN] = 0;
	  *keyPressed = 0;
	  break;
	case ALLEGRO_KEY_LEFT:
	  keys[LEFT] = 0;
	  break;
	case ALLEGRO_KEY_RIGHT:
	  keys[RIGHT] = 0;
	  break;
	case ALLEGRO_KEY_ENTER:
	  keys[ENTER] = 0;
	  break;
	case ALLEGRO_KEY_Q:
	  keys[QUIT] = 0;
	  break;
	}
    }
}
/*initalize the game object*/
void initGameT(gameT *g)
{
  g -> difficulty = 1;/*easiest*/
  g -> topDiff = 10;
  g -> numBullets = 40;
  g -> numEnemyBullets = 10;
  g -> numEnemies = 25;
  g -> numExplosions = 10;
  g -> time = 0;
}

/*when the time comes increase the difficulty*/
void changeDifficulty(gameT *g)
{
  if(g -> difficulty != g -> topDiff)
    g -> difficulty +=1;
  g -> time = 0;
}
/*
not used anymore
*/
void rearrangeObjects(bulletT **enemyBullets,gameT *gT)
{
  int i,j,k;
  bulletT *tmp;
  i = gT -> numEnemyBullets - 5;
  j = gT -> numEnemyBullets;
  tmp = (bulletT *)calloc(sizeof(bulletT),j);
  for(k = 0 ; k < i ; ++k)
    tmp[k] = (*enemyBullets)[k];
  *enemyBullets = tmp;
}
/*when user pressed movement buttons*/
void callMovement(int keys[],enterpriseT *ship,
		  ALLEGRO_DISPLAY_MODE *dispMode)
{
  if(keys[AUTO] == 1 && ship -> state == NORMAL)
      ship -> state = ENT_IDLE;
  else if(keys[AUTO] == 0)
    ship -> state = NORMAL;

  if(ship -> state == NORMAL)
    {
      if(keys[UP] == 1)
	moveEnterpriseUp(ship);
      else if(keys[DOWN] == 1)
	moveEnterpriseDown(ship,dispMode);
      else
	resetEnterpriseFrame(ship,1);
  
      if(keys[LEFT] == 1)
	moveEnterpriseLeft(ship);
      else if(keys[RIGHT] == 1)
	moveEnterpriseRight(ship);
      /* else resetEnterpriseFrame(ship,0); */
    }
}
/*arrange menu things when gamestate is MENU*/
void arrangeMenu(menuT *menu,int keys[],int *gameState)
{
  if(keys[UP] == 1 && menu -> pressed == 0)
    updateMenu(menu,8);
  else if(keys[DOWN] == 1 && menu -> pressed == 0)
    updateMenu(menu,2);
  if(keys[ENTER] == 1)
    {
      switch(menu -> c)
	{
	case 0:
	  *gameState = PLAY;
	  break;
	case 1:
	  *gameState = HIGHSCORE;
	  break;
	case 2:
	  *gameState = EXIT;
	  break;
	}
    }
}
/*draw the statistics user have on left side of the screen*/
void drawStatistic(int shipScore,int shipLives,int shipState,
		   ALLEGRO_BITMAP *liveImage,
		   ALLEGRO_BITMAP *autoImage,
		   ALLEGRO_FONT *font)
{
  al_draw_textf(font,al_map_rgb(177,100,150),10,10,0,"Score :");
  al_draw_textf(font,al_map_rgb(177,100,150),80,10,0,"%d",shipScore);
  al_draw_bitmap(liveImage,120,3,0);
  al_draw_textf(font,al_map_rgb(177,100,150),160,10,0,"%d",shipLives);
  if(shipState != NORMAL)
    al_draw_text(font,al_map_rgb(177,100,150),10,30,0,"Autopilot");
}
