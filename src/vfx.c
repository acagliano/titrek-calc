#include <graphx.h>
#include <tice.h>
#include "equates.h"

void GUI_RenderWarp(char speed){
    char i;
    unsigned int x_origin = xStart + (vWidth>>1);
    unsigned char y_origin = yStart+ (vHeight>>1);
    gfx_SetColor(191);
    for(i=0; i<(speed/4); i++){
        unsigned int x2;
        unsigned char y2;
        char r = randInt(0,3); //assuming rand() generates a float between 0.0 and 1.0
        if (r > 1){
            x2 = (r == 3) ? xStart : xStart + vWidth;
            y2 = randInt(yStart, yStart + vHeight);
        } else {
            y2 = (r == 1) ? yStart : yStart + vHeight;
            x2 = randInt(xStart, xStart + vWidth);
        }
        gfx_Line(x_origin, y_origin, x2, y2);
    }
    gfx_FillCircle(x_origin, y_origin, speed / 6 + 5);
}


