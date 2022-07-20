#include <graphx.h>
#include <fontlibc.h>

void lcars_DrawPercentGraphic(
    const char* title,
    uint24_t xstart,
    uint8_t ystart,
    uint24_t max_width,
    uint8_t bar_height,
    uint8_t percent,
    uint8_t border_color,
    uint8_t border_width,
    uint8_t active_color,
    uint8_t inactive_color){
    
    gfx_SetColor(0);
    gfx_FillRectangle(xstart-border_width, ystart-border_width-10, (border_width<<1)+max_width, (border_width<<1)+bar_height+10);
    
    // draw title
    if(title != NULL){
        fontlib_SetForegroundColor(255);
        fontlib_SetNewlineOptions(FONTLIB_PRECLEAR_NEWLINE);
        fontlib_SetCursorPosition(xstart, ystart-border_width-10);
        fontlib_DrawString(title);
        fontlib_SetNewlineOptions(0);
    }
    
    // draw border
    // border extends beyond the max width/height of bar
    gfx_SetColor(border_color);
    for(int i = 0; i<border_width; i++)
        gfx_Rectangle(xstart-i, ystart-i, (i<<1) + max_width, (i<<1) + bar_height);
    
    // draw active section of bar
    uint24_t pixelw = percent * max_width / 100;
    gfx_SetColor(active_color);
    gfx_FillRectangle(xstart, ystart, pixelw, bar_height);
    
    // draw inactive section of bar
    gfx_SetColor(inactive_color);
    gfx_FillRectangle(xstart + pixelw, ystart, xstart + max_width - pixelw, bar_height);
    gfx_BlitRectangle(gfx_buffer, xstart-border_width, ystart-border_width-10, (border_width<<1)+max_width, (border_width<<1)+bar_height+10);
}
