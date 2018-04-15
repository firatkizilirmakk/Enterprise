#include <math.h>
#include "../include/matfuncs.h"

float distance(int x1,int y1,int x2,int y2)
{
  return sqrt(pow((float)(x1-x2),2) + pow((float)(y1-y2),2));
}

float angleToTarget(int x1,int y1,int x2,int y2)
{
  float dX,dY;
  dX = x2-x1;
  dY = y2-y1;
  return atan2(dY,dX);
}
