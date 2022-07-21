#include <graphx.h>
#include <fontlibc.h>

#include "components.h"

uint8_t bar_border_color;
uint8_t bar_active_color;
uint8_t bar_idle_color;

void lcars_SetBarColors(
    uint8_t border_color,
    uint8_t active_color,
    uint8_t idle_color){
    bar_border_color = border_color;
    bar_active_color = active_color;
    bar_idle_color = idle_color;
}

void lcars_DrawLoadBar(
    const char* title,
    uint24_t xstart,
    uint8_t ystart,
    uint24_t max_width,
    uint8_t bar_height,
    uint8_t percent){
    
    // draw title if one is specified
    if(title != NULL){
        gfx_SetColor(0);
        gfx_FillRectangle(xstart, ystart, max_width, 10);
        fontlib_SetForegroundColor(255);
        fontlib_SetCursorPosition(xstart+5, ystart+1);
        fontlib_DrawString(title);
        gfx_BlitRectangle(gfx_buffer, xstart, ystart, max_width, 10);
    }
    
    // delegate to code that draws bar itself
    lcars_DrawBar(xstart, ystart + 11, max_width, bar_height, percent);
    
}


void lcars_DrawBar(
    uint24_t xstart,
    uint8_t ystart,
    uint24_t max_width,
    uint8_t bar_height,
    uint8_t percent){
    
    // draw border
    // border starts at xstart, ystart and is in addition to bar_height/width.
    // plan dimensions accordingly
    gfx_SetColor(bar_border_color);
    gfx_Rectangle(xstart, ystart, max_width+2, bar_height+2);
    
    // draw active section of bar
    uint24_t pixelw = percent * max_width / 100;
    gfx_SetColor(bar_active_color);
    gfx_FillRectangle(xstart+1, ystart+1, pixelw+1, bar_height);
    
    // draw inactive section of bar
    gfx_SetColor(bar_idle_color);
    gfx_FillRectangle(xstart + pixelw + 1, ystart + 1, xstart + 1 + max_width - pixelw, bar_height);
    gfx_BlitRectangle(gfx_buffer, xstart, ystart, max_width+2, bar_height+2);
}
