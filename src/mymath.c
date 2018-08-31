#include "mymath.h"
#include <string.h>
#include <math.h>
const char cosLUT[256] = {127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 111, 110, 108, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 84, 82, 80, 77, 75, 72, 70, 67, 64, 62, 59, 56, 53, 50, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 17, 14, 11, 8, 5, 2, 0, -3, -7, -10, -13, -16, -19, -22, -25, -28, -31, -34, -37, -40, -43, -46, -49, -52, -55, -58, -60, -63, -66, -68, -71, -74, -76, -79, -81, -83, -86, -88, -90, -92, -94, -96, -98, -100, -102, -104, -106, -107, -109, -111, -112, -114, -115, -116, -117, -118, -120, -121, -121, -122, -123, -124, -124, -125, -125, -126, -126, -126, -126, -126, -126, -126, -126, -126, -126, -125, -125, -124, -124, -123, -122, -121, -121, -120, -118, -117, -116, -115, -114, -112, -111, -109, -107, -106, -104, -102, -100, -98, -96, -94, -92, -90, -88, -86, -83, -81, -79, -76, -74, -71, -68, -66, -63, -60, -58, -55, -52, -49, -46, -43, -40, -37, -34, -31, -28, -25, -22, -19, -16, -13, -10, -7, -3, 0, 2, 5, 8, 11, 14, 17, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 50, 53, 56, 59, 62, 64, 67, 70, 72, 75, 77, 80, 82, 84, 87, 89, 91, 93, 95, 97, 99, 101, 103, 105, 107, 108, 110, 111, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126, 127};

long r_GetDistance(int xdiff, int ydiff, int zdiff){
    long distance = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
    return distance;
}

signed char byteCos(unsigned char x){
    return cosLUT[x];
}

signed char byteSin(unsigned char x){
    return byteCos(x-64);
}

char r_ArcTan(short numerator, short denominator){
    
}




void AnglesToVectors(Position_t *pos){
    unsigned char xzangle = pos->angles.xz, yangle = pos->angles.y;
    pos->vectors.x = byteCos(xzangle - 64) * byteCos(yangle);
    pos->vectors.z = byteSin(xzangle - 64) * byteCos(yangle);
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
