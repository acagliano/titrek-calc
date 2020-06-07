#include <graphx.h>
#include <stdint.h>
#include <tice.h>
#include "../gfx/trekgui.h"
#include "../classes/ships.h"
#include "../gfx/moduleicons.h"

#define MODICON_START 17
gfx_rletsprite_t* modicons[TARG_SENS + 1] = {
    0
};

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t border, uint8_t bg, uint8_t bar){
    gfx_SetColor(border);
    gfx_Rectangle(x, y-1, length, 10);
    length -= 2;
    gfx_SetColor(bg);
    gfx_FillRectangle(x + 1, y, length, 8);
    gfx_SetColor(bar);
    if(health <= 50) gfx_SetColor(229);
    if(health <= 25) gfx_SetColor(192);
    gfx_FillRectangle(x + 1, y, health * length / 100, 8);
    gfx_SetColor(255);
}

void gfx_InitModuleIcons(void){
    uint24_t src = MODICON_START, ct = trekgui_num - MODICON_START, i;
    
    for(i = 0; i < ct; i++){
        modicons[i] = ((gfx_rletsprite_t*)trekgui[src++]);
    }
}

uint8_t gfx_RenderSplash(gfx_rletsprite_t* splash, bool net){
    uint24_t text_x = 60;
    uint8_t text_y = 140;
    uint24_t key = 0;
    uint8_t selected = 0;
    gfx_ZeroScreen();
    gfx_RLETSprite(splash, 320 - 260, 20);
    gfx_SetTextFGColor(229);
    gfx_PrintStringXY("Play Game", text_x, text_y);
    gfx_PrintStringXY("Settings", text_x, text_y + 15);
    gfx_PrintStringXY("About Game", text_x, text_y + 30);
    gfx_PrintStringXY("Quit Game", text_x, text_y + 45);
    if(!net) {
        gfx_SetTextFGColor(224);
        gfx_PrintStringXY("Networking disabled!", text_x + 100, text_y + 45);
    }
    
    while((key = os_GetCSC()) != sk_Enter){
        if(key){
            gfx_SetColor(0);
            gfx_FillCircle(text_x - 10, selected * 15 + text_y + 3, 4);
        }
        if(key == sk_Up) selected -= (selected > 0);
        if(key == sk_Down) selected += (selected < 3);
        if(key == sk_Clear) {selected = 3; break;}
        gfx_SetColor(229);
        gfx_FillCircle(text_x - 10, selected * 15 + text_y + 3, 4);
        gfx_BlitBuffer();
    }
    return selected;
}

 void gfx_RenderWindow(uint24_t x, uint8_t y, uint24_t width, uint8_t height, uint8_t borderColor, uint8_t windowColor, uint8_t borderSize){
    gfx_SetColor(borderColor);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(windowColor);
    gfx_FillRectangle(x + borderSize, y + borderSize, width - (borderSize<<1), height - (borderSize<<1));
 }
