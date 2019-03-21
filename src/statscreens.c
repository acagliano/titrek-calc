#include "classes/ships.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include <graphx.h>
#include <compression.h>

void Screen_UISysStats(module_t* system, unsigned char count){
    unsigned char i;
    gfx_PrintStringXY("[SYSTEM STATS]", viewer_x + 80, viewer_y);
    for(i = 0; i < count; i++){
        // loop module health display
        module_t* module = &system[i];
        gfx_SetTextXY(viewer_x, (i + 1) * 10 + viewer_y);
        if(!module->techclass){
            gfx_PrintString("module unloaded");
        }
        else {
            signed int health = health_GetHealthPercent(&module->health);
            gfx_PrintUInt(health, 3);
        }
    }
    return;
}















void Screen_Background(gfx_sprite_t** sprites, unsigned char active) {
    gfx_Sprite(sprites[0], 0, 0);
    gfx_Sprite(sprites[1], 160, 0);
    gfx_SetColor(255);
    gfx_FillRectangle(active*64, 216, 64, 20);
    gfx_PrintStringXY("View", 20, lcars_texty);
    gfx_PrintStringXY("Tact", 80, lcars_texty);
    gfx_PrintStringXY("Stat", 145, lcars_texty);
    gfx_PrintStringXY("Power", 205, lcars_texty);
    gfx_PrintStringXY("Invent", 265, lcars_texty);
}
