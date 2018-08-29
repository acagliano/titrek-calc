
#ifndef mymath_h
#define mymath_h
#include "datatypes/playerdata.h"

long r_GetDistance(int xdiff, int ydiff, int zdiff);
signed char byteCos(unsigned char x);
signed char byteSin(unsigned char x);
void AnglesToVectors(Position_t *pos);
char lcars_GetIntLength(char value);
unsigned int text_GetCenterX(char* text, int viewer_width);

extern const char cosLUT[256];




#endif
