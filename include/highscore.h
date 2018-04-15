#ifndef HIGHSCORE_H
#define HIGHSCORE_H

void initScores(highScoreT *scores);
void showScores(highScoreT *scores,ALLEGRO_FONT *font);
int readScores(char *filename,highScoreT *scores);
int compStructs(const void *s1,const void *s2);
void writeScores(char *filename,highScoreT *scores);
void syncScores(char *filename,highScoreT *scores,char *name,int score);
#endif
