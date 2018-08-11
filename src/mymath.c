#include "mymath.h"

long r_GetDistance(short xdiff, short ydiff, short zdiff){
    long distance = xdiff^2 + ydiff^2 + zdiff^2;
    return distance;
}

int8_t byteCos(uint8_t x){
    if (x&128) return ~cosLUT[x&127];
    return cosLUT[x];
}

signed char byteSin(unsigned char x){
    return byteCos(x-64);
}

char r_ArcTan(short numerator, short denominator){
    
}
