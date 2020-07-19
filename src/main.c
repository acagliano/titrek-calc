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
#include "equates.h"
#include "network/controlcodes.h"
#include "network/network.h"

#include "classes/ships.h"
#include "classes/modules.h"
#include "classes/screens.h"
#include "classes/settings.h"

#include "rendering/screens.h"
#include "rendering/errors.h"
#include "rendering/gui.h"

#include "gfx/TrekGFX.h"
#include "gfx/internal.h"

#include "asm/exposure.h"

// USB Libraries
#include <usbdrvce.h>

#define DEBUG
#undef NDEBUG
#include <debug.h>


#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

char *settingsappv = "TrekSett";
char *TEMP_PROGRAM = "_";
char *MAIN_PROGRAM = "TITREK";
char version[] = {0, 0, 91};
char versionstr[12] = {0};
ship_t Ship = {0};
selected_t select = {0, 0};
gfx_rletsprite_t* gfx_sprites;
bool debug = 0;
gfx_UninitedRLETSprite(splash, splash_size);
gfx_UninitedRLETSprite(err_icon, icon_internalerr_size);
flags_t gameflags = {0};
settings_t settings = {0};
uint24_t ticknum = 0;
moduleinfo_t ModuleInfo = {};

ti_var_t update_fp = 0;

/* Main Menu */

void PlayGame(void);

void MainMenu(void) {
    uint8_t opt = 0;
    while(1){
        opt = gfx_RenderSplash(splash);
        if(opt == OPT_PLAY) {gameflags.loopgame = 1; PlayGame();}
        if(opt == OPT_QUIT) {gameflags.exit = 1; break;}
        if(opt == OPT_ABOUT) {
            gfx_ZeroScreen();
            gfx_SetColor(239);
            gfx_PrintStringXY("## About Project TI-Trek ##", 5, 5);
            gfx_PrintStringXY("A multiplayer space combat game", 10, 20);
            gfx_PrintStringXY("for the TI-84+ CE!", 10, 30);
            gfx_PrintStringXY("_Authors_", 10, 40);
            gfx_PrintStringXY("ACagliano - lead, client", 15, 50);
            gfx_PrintStringXY("beck - lead, server", 15, 60);
            gfx_PrintStringXY("command - lead, USB bridge", 15, 70);
            gfx_PrintStringXY("(c) 2019, Project TI-Trek", 5, 230);
            gfx_BlitBuffer();
            while(!kb_IsDown(kb_KeyClear)) kb_Scan();
        }
        if(opt == OPT_SETTINGS){}
    }
}

int main(void) {
    ti_var_t savefile;
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    zx7_Decompress(splash, splash_compressed);
    zx7_Decompress(err_icon, icon_internalerr_compressed);
    if(savefile = ti_Open(settingsappv, "r")){
        ti_Read(&settings, sizeof(settings), 1, savefile);
        ti_Close(savefile);
    } else { set_defaults(); }

    if(!ntwk_init()) goto error;

    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    strify_version(versionstr, version);
    do {
        MainMenu();
    } while(!gameflags.exit);
error:
    usb_Cleanup();
    write_settings();
    gfx_End();
    pgrm_CleanUp();
    return 0;
}


void PlayGame(void){
    uint16_t screen = 0;
    if(!gameflags.network) return;
    if(!gui_Login()) {
        dbg_sprintf(dbgout, "Failed to login\n");
        return;
    }
    if(!TrekGFX_init()) {
        dbg_sprintf(dbgout, "Failed to init graphics\n");
        return;
    }
    gfx_InitModuleIcons();
    do {
        /* A buffer to store bytes read by the serial library */
        size_t bytes_read;
        sk_key_t key = getKey();
        Screen_RenderUI(screen, &select);
        if(!gameflags.logged_in) gui_NetworkErrorResponse(3, 6, false);
        gfx_BlitBuffer();
        if(key == sk_Clear){
            if(gameflags.logged_in){
                if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
                else ntwk_send_nodata(DISCONNECT);
            }
            else gameflags.loopgame = false;
        }
        if(key == sk_Stat) {debug = true;}
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
        if(key == sk_Down) {
            char i;
            switch(screen){
                case SCRN_TACT:
                    for(i = select.tactical + 1; i < (MAX_MODULES - 1); i++){
                        int type = Ship.system[i].modclass;
                        if( type == mTactical ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case SCRN_MAINS:
                    for(i = select.mains + 1; i < (MAX_MODULES - 1); i++){
                        int type = Ship.system[i].modclass;
                        if( type == mSystem ){
                            select.mains = i;
                            break;
                        }
                    }
                    break;
                }
            }
        if(key == sk_Up){
            char i;
            switch(screen){
                case SCRN_TACT:
                    for(i = select.tactical - 1; i >= 0; i--){
                        int type = Ship.system[i].modclass;
                        if( type == mTactical ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case SCRN_MAINS:
                    for(i = select.mains - 1; i >= 0; i--){
                        int type = Ship.system[i].modclass;
                        if( type == mSystem ){
                            select.mains = i;
                            break;
                        }
                    }
                    break;
                }
            }
            /*
        if(gameflags.logged_in){
            if(ticknum % settings.limits.chunk_refresh == 0) {
                ntwk_send(REQCHUNK,
                    PS_VAL(Ship.rotate.yaw),
                    PS_VAL(Ship.rotate.pitch),
                    PS_VAL(Ship.rotate.roll));
            }
            if(ticknum % settings.limits.entity_refresh == 0) {
                ntwk_send(REQENTITY,
                    PS_VAL(Ship.rotate.yaw),
                    PS_VAL(Ship.rotate.pitch),
                    PS_VAL(Ship.rotate.roll));
            }
        }
        */
        ntwk_process();
        gfx_SwapDraw();
        ticknum++;
    } while(gameflags.loopgame && gameflags.network);
    dbg_sprintf(dbgout, "%u%u\n", gameflags.loopgame, gameflags.network);
}
