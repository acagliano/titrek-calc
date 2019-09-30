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
#include "classes/screens.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
/* Put your function prototypes here */
#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

ship_t Ship = {0};
selected_t select = {0, SYS_MAX};

void main(void) {
    uint16_t screen = 0;
    bool loopgame = true;
    unsigned int i;
    ti_var_t appvar;
    gfx_rletsprite_t* gfx_sprites = malloc(trekgui_uncompressed_size);
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    if(!gfx_sprites) return;
    if(!(appvar = ti_Open("trekgui", "r"))) return;
    zx7_Decompress(gfx_sprites, ti_GetDataPtr(appvar));
    trekgui_init(gfx_sprites);
    ti_CloseAll();
    int_Disable();
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    module_SetHealthMax(&Ship.hull.health, 1000);
    for(i = 0; i < SYS_MAX; i++){
       // Ship.system[i].techclass = MC_SYSTEM;
        Ship.system[i].assigned = true;
        Ship.system[i].online = true;
        Ship.system[i].techtype = i;
        module_SetHealthMax(&Ship.system[i].health, 50);
        module_SetPowerMax(&Ship.system[i].power);
    }
    for(i = 0; i < 2; i++){
      //  Ship.shield[i].techclass = MC_TACTICAL;
        Ship.tactical[i].unlocked = true;
        Ship.tactical[i].assigned = true;
        Ship.tactical[i].online = true;
        Ship.tactical[i].techtype = SHIELD;
        module_SetHealthMax(&Ship.tactical[i].health, 50);
        module_SetPowerMax(&Ship.tactical[i].power);
        Ship.tactical[i].data.mod_shields.resistance = 10;
    }
    do {
        unsigned char key = os_GetCSC();
        Screen_RenderUI(screen, &Ship, &select);
        if(key == sk_Clear){
            if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
            else loopgame = false;
        }
        if(key == sk_Yequ)
            screen = (screen == SCRN_SENS) ? SCRN_OFF : SCRN_SENS;
        if(key == sk_Window)
            screen = (screen == SCRN_TACT) ? SCRN_OFF : SCRN_TACT;
        if(key == sk_Zoom)
            screen = (screen == SCRN_MAINS) ? SCRN_OFF : SCRN_MAINS;
        if(key == sk_Trace)
            screen = (screen == SCRN_TRANSPORT) ? SCRN_OFF : SCRN_TRANSPORT;
        if(key == sk_Graph)
            screen = (screen == SCRN_CARGO) ? SCRN_OFF : SCRN_CARGO;
        if(key == sk_Enter){
            if((screen == SCRN_MAINS) || (screen == SCRN_TACT))
                screen = setbits(screen, SCRN_INFO);
        }
        if(key == sk_Square){
            char i;
            char modnum = randInt(0, SYS_MAX - 1);
            char randnum = randInt(5, 10);
            for(i = 0; i < TACT_MAX; i++){
                module_t *shield = &Ship.tactical[i];
                if((shield->techtype == SHIELD) && shield->online){
                    int health = health_GetHealthPercent(&shield->health);
                    int drv = shield->data.mod_shields.resistance;
                    drv = health * drv / 100;
                    health_DamageModule(&shield->health, -(randnum));
                    randnum -= drv;
                }
            }
            health_DamageModule(&Ship.hull.health, -(randnum));
            health_DamageModule(&Ship.system[modnum].health, -(randnum));
        }
        if(key == sk_Down) {
            char i;
            switch(screen){
                case 1:
                    for(i = select.tactical + 1; i < (TACT_MAX - 1); i++){
                        int type = Ship.tactical[i].techtype;
                        if( type == SHIELD || type == ARMOR ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case 3:
                    if(select.mains < (SYS_MAX - 1)) select.mains++;
                    break;
            }
        }
        if(key == sk_Up){
            char i;
            switch(screen){
                case 1:
                    for(i = select.tactical - 1; i >= 0; i--){
                        int type = Ship.tactical[i].techtype;
                        if( type == SHIELD || type == ARMOR ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case 3:
                    if(select.mains > 0) select.mains--;
                    break;
            }
        }
            
    } while(loopgame == true);
    free(gfx_sprites);
    gfx_End();
    int_Enable();
    pgrm_CleanUp();
    return;
}

/* Put other functions here */
/* Draw text on the homescreen at the given X/Y location */
