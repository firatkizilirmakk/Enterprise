#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "../include/objects.h"
#include "../include/main.h"

void initScores(highScoreT *scores)
{
  int i;
  for(i = 0 ; i < 11 ; ++i)
    {
      scores[i].score = 0;
      strcpy(scores[i].name,"empty");
    }
}
/*show the scores on the screen*/
void showScores(highScoreT *scores,ALLEGRO_FONT *font)
{
  ALLEGRO_COLOR uncolored = al_map_rgb(255,177,177);
  int i;
  for(i = 0 ; i < 10 ; ++i)
    {
      al_draw_textf(font,uncolored,
		    WIDTH / 2 - 100,200 + i * 20,
		    0,"%s       %d",scores[i].name,scores[i].score);
    }
}
/*if file is empty return 0 and 1 otherwise*/
int readScores(char *filename,highScoreT *scores)
{
  FILE *fp;
  int i;
  fp = fopen(filename,"rb");
  if(fp != NULL)
    {
      /*empty file*/
      if(fread(&scores[0],sizeof(highScoreT),1,fp) != 1)
	return 0;
      for(i = 1 ; i < 10 ; ++i)
	fread(&scores[i],sizeof(highScoreT),1,fp);
    }
  fclose(fp);
  return 1;
}
/*function for qsort*/
int compStructs(const void *s1,const void *s2)
{
  highScoreT *score1 = (highScoreT *)s1;
  highScoreT *score2 = (highScoreT *)s2;
  return (score2 -> score - score1 -> score);
}
/*write scores to the file*/
void writeScores(char *filename,highScoreT *scores)
{
  FILE *fp;
  int i;
  fp = fopen(filename,"wb");
  if(fp != NULL)
    {
      for(i = 0 ; i < 10 ; ++i)
	fwrite(&scores[i],sizeof(highScoreT),1,fp);
    }
  fclose(fp);
}
/*syncronize the scores*/
void syncScores(char *filename,highScoreT *scores,char *name,int score)
{
  if(readScores(filename,scores) == 0)
    initScores(scores);
  scores[10].score = score;
  strcpy(scores[10].name,name);
  qsort(scores,11,sizeof(highScoreT),compStructs);
  writeScores(filename,scores);
}
