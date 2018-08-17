#include <graphx.h>
#include <tice.h>
#include "equates.h"

void GUI_RenderWarp(char speed){
    char i;
    gfx_SetColor(191);
    for(i=0; i<(speed/4); i++){
        unsigned int x2;
        unsigned char y2;
        char r = randInt(0,3); //assuming rand() generates a float between 0.0 and 1.0
        if (r > 1){
            x2 = (r == 3) ? xStart : xStart + 272;
            y2 = randInt(yStart, yStart + 162);
        } else {
            y2 = (r == 1) ? yStart : yStart + 162;
            x2 = randInt(xStart, xStart + 272);
        }
        gfx_Line(xStart+136, yStart+81, x2, y2);
    }
    gfx_FillCircle(xStart+136, yStart+81, speed / 6 + 5);
}


