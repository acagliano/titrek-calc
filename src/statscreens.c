#include "classes/ships.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include <graphx.h>
#include <compression.h>

void Screen_UISysStats(module_t* system, unsigned char count){
    unsigned char i;
    for(i = 0; i < count; i++){
        // loop module health display
    }
}















void Screen_Background(unsigned char active) {
    gfx_sprite_t *decompressed, *flipped;
    decompressed = gfx_MallocSprite(shipinterior_width, shipinterior_height);
    flipped = gfx_MallocSprite(shipinterior_width, shipinterior_height);
    zx7_Decompress(decompressed, shipinterior_compressed);
    gfx_FlipSpriteY(decompressed, flipped);
    gfx_Sprite(decompressed, 0, 0);
    gfx_Sprite(flipped, 160, 0);
    free(decompressed);
    free(flipped);
    gfx_SetColor(255);
    gfx_FillRectangle(active*64+2, 216, 62, 20);
    gfx_PrintStringXY("View", 20, lcars_texty);
    gfx_PrintStringXY("Tact", 80, lcars_texty);
    gfx_PrintStringXY("Stat", 145, lcars_texty);
    gfx_PrintStringXY("Power", 205, lcars_texty);
    gfx_PrintStringXY("Invent", 265, lcars_texty);
}
