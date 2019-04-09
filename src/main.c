//--------------------------------------
// Program Name: Star Trek Multplayer
// Author: Anthony Cagliano
// License:
// Description:
//--------------------------------------

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <intce.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h, debug.h
#include <fileioc.h>
#include <graphx.h>
#include <compression.h>
#include <keypadc.h>

// Game Data Types (Structure Definitions)

#include "classes/ships.h"
#include "classes/coords.h"
#include "classes/tech.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
/* Put your function prototypes here */
void DrawFrame(gfx_sprite_t **sprites, unsigned char screen);

ship_t Ship = {0};
gfx_sprite_t *sprites[trekgui_num + 1];

void main(void) {
    unsigned char screen = 0;
    bool loopgame = true;
    unsigned int i;
    gfx_Begin();
    if(!trekgui_init()) return;
    sprites[0] = gfx_MallocSprite(shipinterior_width, shipinterior_height);
    sprites[1] = gfx_MallocSprite(shipinterior_width, shipinterior_height);
    zx7_Decompress(sprites[0], shipinterior_compressed);
    gfx_FlipSpriteY(sprites[0], sprites[1]);
    int_Disable();
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    for(i = 0; i < SYS_MAX; i++){
        Ship.system[i].techclass = MC_SYSTEM;
        Ship.system[i].data.techtype = i;
        module_SetHealthMax(&Ship.system[i].health);
        module_SetPowerMax(&Ship.system[i].power);
    }
    do {
        unsigned char key = os_GetCSC();
        DrawFrame(sprites, screen);
        if(key == sk_Clear) loopgame = false;
        if(key == sk_Yequ) screen = 0;
        if(key == sk_Window) screen = 1;
        if(key == sk_Zoom) screen = 2;
        if(key == sk_Trace) screen = 3;
        if(key == sk_Graph) screen = 4;
    } while(loopgame == true);
    free(sprites[0]);
    free(sprites[1]);
    gfx_End();
    int_Enable();
    pgrm_CleanUp();
    return;
}

void DrawFrame(gfx_sprite_t **sprites, unsigned char screen){
    Screen_Background(sprites, screen);
    gfx_SetTextFGColor(255);
    switch(screen){
        case 1:
            Screen_UIModuleStats("[TACTICAL CONTROL]", &Ship.tactical[0], TACT_MAX);
            break;
        case 2:
            Screen_UIModuleStats("[MAIN SYS CONTROL]", &Ship.system[0], SYS_MAX);
            break;
        case 3:
            //Screen_UIRepairSys(&Ship.system[0], SYS_MAX + TACT_MAX);
            break;
    }
    gfx_BlitBuffer();
    gfx_SetTextFGColor(0);
    return;
}
/* Put other functions here */
/* Draw text on the homescreen at the given X/Y location */
