
#ifndef mymath_h
#define mymath_h
#include "datatypes/playerdata.h"
#include "datatypes/mapdata.h"

unsigned long r_GetDistance(int xdiff, int ydiff, int zdiff);
signed char byteCos(unsigned char x);
signed char byteSin(unsigned char x);
char byteATan(long non_x, long x);
void AnglesToVectors(Position_t *pos);
char lcars_GetIntLength(char value);
unsigned int text_GetCenterX(char* text, int viewer_width);
char calcSpriteScale(unsigned long distance, unsigned int render_distance);
char AngleOpsBounded(char angle, char operand);

extern const char cosLUT[72];

// Heapsort Stuff
int gt(renderitem_t x,renderitem_t y);
void popheap(renderitem_t* arr,int size);
void heapify(renderitem_t* arr,int size);
void heapsort(renderitem_t* arr,int size);

#define TO_DEG M_PI/180

#endif
