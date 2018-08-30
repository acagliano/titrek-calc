#include "mymath.h"
#include <string.h>
const char cosLUT[256] = {127,127,127,127,126,126,126,125,125,124,123,122,121,121,120,118,117,116,115,113,112,110,109,107,105,104,102,100,98,96,94,92,90,87,85,83,80,78,75,73,70,68,65,62,59,57,54,51,48,45,42,39,36,33,30,27,24,21,18,15,12,9,5,2,-1,-4,-7,-10,-13,-16,-19,-23,-26,-29,-32,-35,-38,-41,-44,-47,-50,-52,-55,-58,-61,-63,-66,-69,-71,-74,-77,-79,-81,-84,-86,-88,-91,-93,-95,-97,-99,-101,-103,-105,-106,-108,-110,-111,-113,-114,-115,-117,-118,-119,-120,-121,-122,-123,-124,-124,-125,-125,-126,-126,-127,-127,-127,-127};

long r_GetDistance(int xdiff, int ydiff, int zdiff){
    long distance = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
    return distance;
}

signed char byteCos(unsigned char x){
    if (x&128) return ~cosLUT[x&127];
    return cosLUT[x];
}

signed char byteSin(unsigned char x){
    return byteCos(x-64);
}

char r_ArcTan(short numerator, short denominator){
    
}


void AnglesToVectors(Position_t *pos){
    unsigned char xzangle = pos->angles.xz, yangle = pos->angles.y;
    pos->vectors.x = byteCos(xzangle) * byteCos(yangle);
    pos->vectors.z = byteSin(xzangle) * byteCos(yangle);
    pos->vectors.y = byteSin(yangle);
    //pos->vectors[2] = z vector
}

char lcars_GetIntLength(char value){
    return 1 + (value > 9) + (value > 99);
}

unsigned int text_GetCenterX(char* text, int viewer_width){
    unsigned char length = strlen(text) + 2;
    unsigned char pixellen = length * 5 / 2;
    return (viewer_width>>1) - pixellen;
}
