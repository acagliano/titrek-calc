#include <stdbool.h>
#include <graphx.h>
#include "windows.h"
#include "text.h"

void window_DrawWindow(window_t* window, uint8_t bgcolor, uint8_t bordercolor, uint8_t borderwidth){
    gfx_SetColor(bgcolor);
    gfx_FillRectangle(window->x, window->y, window->w, window->h);
    gfx_SetColor(bordercolor);
    for(int i=borderwidth; i>0; i--)
        gfx_Rectangle(window->x-i, window->y-i, (i<<1) + window->w, (i<<1) + window->h);
}

void window_DrawTitle(window_t* window, uint8_t titlebgcolor, uint8_t titlefontcolor, const char* title, bool largetext){
    uint8_t fontheight = 8;
    if(largetext) {
        gfx_SetTextScale(2, 2);
        fontheight<<=1;
    }
    uint8_t start_y = window->y - (10 + fontheight);
    uint24_t font_x = window->x + ((window->w - gfx_GetStringWidth(title))>>1);
    gfx_SetColor(titlebgcolor);
    gfx_SetTextFGColor(titlefontcolor);
    gfx_FillRectangle(window->x, start_y, window->w, 10+fontheight);
    gfx_PrintStringXY(title, font_x, window->y - (5 + fontheight));
    gfx_SetTextScale(1,1);
}

void box_ParentToWindow(box_t* box, window_t* window){
    box->x = (box->x) ? box->x : window->x;
    box->y = (box->y) ? box->y : window->y;
    box->w = (box->w) ? box->w : window->w;
    box->h = (box->h) ? box->h : window->h;
}

void box_DrawBox(box_t* box, uint8_t bgcolor, uint8_t bordercolor, uint8_t borderwidth){
    window_DrawWindow((window_t*)box, bgcolor, bordercolor, borderwidth);
}

void box_PrintStringXY(box_t* box, const char* string, uint24_t x, uint8_t y, uint8_t color){
    x += box->x;
    y += box->y;
    uint8_t oldcolor = gfx_SetTextFGColor(color);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextFGColor(oldcolor);
}

void box_SetTextXY(box_t* box, uint24_t x, uint8_t y){
    x += box->x;
    y += box->y;
    gfx_SetTextXY(x, y);
}


void box_RenderSprite(box_t* box, gfx_rletsprite_t* sprite, uint24_t x, uint8_t y){
    x += box->x;
    y += box->y;
    gfx_RLETSprite(sprite, x, y);
}
