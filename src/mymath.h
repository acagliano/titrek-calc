
#ifndef mymath_h
#define mymath_h
#include "datatypes/playerdata.h"
#include "datatypes/mapdata.h"

unsigned long r_GetDistance(int xdiff, int ydiff, int zdiff);
signed short byteCos(unsigned char x);
signed short byteSin(unsigned char x);
unsigned char byteATan(long non_x, long x);
void AnglesToVectors(Position_t *pos);
char lcars_GetIntLength(char value);
unsigned int text_GetCenterX(char* text, int viewer_width);
char calcSpriteScale(unsigned long distance, unsigned int render_distance);
char AngleOpsBounded(char angle, char operand);
char compareAngles(unsigned char x, unsigned char y);

extern const short cosLUT[256];
//extern const char sinLUT[64];
extern const int arctanLUT[64];

// Heapsort Stuff
int gt(renderitem_t x,renderitem_t y);
void popheap(renderitem_t* arr,int size);
void heapify(renderitem_t* arr,int size);
void heapsort(renderitem_t* arr,int size);

#define TO_DEG M_PI/180

#endif
