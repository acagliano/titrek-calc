#include <graphx.h>
#include <stdint.h>
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
