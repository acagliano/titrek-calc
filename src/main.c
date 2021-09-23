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
#include "flags.h"
#include "versioning.h"
#include "network/controlcodes.h"
#include "network/network.h"

#include "classes/ships.h"
#include "classes/modules.h"
#include "classes/settings.h"

#include "lcars/screens.h"
#include "lcars/errors.h"
#include "lcars/gui.h"
#include "lcars/caching/imgcaching.h"
#include "lcars/effects/particles.h"
#include "lcars/keybinds/keyfuncs.h"

#include "gfx/TrekGFX.h"
#include "gfx/internal.h"

#include "asm/exposure.h"

// USB Libraries
#include <usbdrvce.h>

#define DEBUG
#undef NDEBUG
#include <debug.h>

char *settingsappv = "TrekSett";
char *keyappv = "trekkey";
char *TEMP_PROGRAM = "_";
char *MAIN_PROGRAM = "TITREK";
ship_t Ship = {0};
selected_t select = {0, 0};
uint16_t screen = 0;
gfx_rletsprite_t* gfx_sprites;
gfx_UninitedRLETSprite(splash, splash_size);
gfx_UninitedRLETSprite(err_icon, icon_error_size);
gfx_UninitedRLETSprite(icon_netup, icon_networkup_size);
gfx_UninitedRLETSprite(icon_netdown, icon_networkdown_size);
gfx_UninitedRLETSprite(log_error, log_error_size);
gfx_UninitedRLETSprite(log_info, log_info_size);
gfx_UninitedRLETSprite(log_debug, log_debug_size);
gfx_UninitedRLETSprite(log_server, log_server_size);
bool debug = 0;
flags_t gameflags = {0};
net_flags_t netflags = {0};
settings_t settings = {0};
uint24_t ticknum = 0;
moduleinfo_t ModuleInfo = {0};
uint24_t ntwk_inactive_clock = 0;
uint24_t ntwk_inactive_disconnect = 0;
bridge_config_t bridge_config = {0};
particles_t particles[MAX_PARTICLES] = {0};
engine_ref_t engine_ref = {0};
uint8_t playgame_return = 0;
bool full_redraw = true;
bool gfx_loaded = false;
uint8_t errors = 0;

ti_var_t update_fp = 0;

/* Main Menu */

uint8_t PlayGame(void);
void ServerSelect(void);

void MainMenu(void) {
    uint8_t opt = 0;
    while(1){
        opt = gfx_RenderSplash(splash, playgame_return);
        if(opt == OPT_PLAY) ServerSelect();
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
            while(kb_IsDown(kb_KeyClear)) kb_Scan();
            opt = 0;
        }
        if(opt == OPT_SETTINGS){
            uint24_t start_x = 20, start_y = 5, i, sel = 0;
            bool firstrender = true;
            sk_key_t key;
            do {
                key = getKey();
                if(key == sk_Down) sel = sel + (sel < (NUM_SETTINGS-1));
                if(key == sk_Up) sel = sel - (sel > 0);
                if(key == sk_Left) gfx_AlterSettingOpt(sel, -1);
                if(key == sk_Right) gfx_AlterSettingOpt(sel, 1);
                if(key || firstrender){
                    gfx_ZeroScreen();
                    for(i = 0; i < NUM_SETTINGS; i++)
                        gfx_RenderSettingOpt(i, sel, start_x, i * 16 + start_y);
                    gfx_BlitBuffer();
                    firstrender = false;
                }
    
            } while (key != sk_Clear);
            while(kb_AnyKey()) kb_Scan();
            opt = 0;
        }
    }
}

void ServerSelect(void){
    uint8_t i, selected = 0;
    bool firstrender = true;
    sk_key_t key;
    if(gameflags.gfx_error) return;
    do {
        key = getKey();
        if(key == sk_Down) selected += (selected < 9);
        if(key == sk_Up) selected -= (selected > 0);
        if(key == sk_Clear) break;
        if((key == sk_Del) && (selected != 0)){
            prompt_for("Add Server:", settings.servers[selected], 49, 20, 210, 0);
        }
        if(key || firstrender){
            gfx_ZeroScreen();
            gfx_RenderMenuTitle("Server List", 3, 5);
            gfx_RenderMenu(settings.servers, 10, selected, 5, 20, 200, 160);
            gfx_SetTextFGColor(255);
            gfx_PrintStringXY("[Enter] Select", 210, 30);
            gfx_PrintStringXY("[Del] Edit/Add", 210, 40);
            gfx_BlitBuffer();
            firstrender = false;
        }
        if(key == sk_Enter){
            if(settings.servers[selected][0]){
                bridge_config.server = selected;
                playgame_return = PlayGame();
                break;
            }
        }
       
    } while (1);
}

int main(void) {
    ti_var_t savefile;
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    if((savefile = ti_Open(settingsappv, "r")) && (ti_GetSize(savefile) == sizeof(settings))){
        ti_Read(&settings, sizeof(settings), 1, savefile);
        ti_Close(savefile);
    }
    if(savefile = ti_Open(settingsappv, "r")){
        
    } else { set_defaults(); }

    check_import_login_key();
    zx7_Decompress(err_icon, icon_error_compressed);
    zx7_Decompress(splash, splash_compressed);
    zx7_Decompress(icon_netup, icon_networkup_compressed);
    zx7_Decompress(icon_netdown, icon_networkdown_compressed);
    zx7_Decompress(log_error, log_error_compressed);
    zx7_Decompress(log_info, log_info_compressed);
    zx7_Decompress(log_debug, log_debug_compressed);
    zx7_Decompress(log_server, log_server_compressed);
    if(!ntwk_init()) goto error;
    
    gfx_VersionCheck();
    
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    strify_version(versionstr, version);
    do {
        MainMenu();
    } while(!gameflags.exit);
error:
    //cache_purge();
    write_settings();
    gfx_End();
    usb_Cleanup();
    pgrm_CleanUp();
    return 0;
}

uint24_t arr_sum(uint24_t arr[], uint24_t ct){
    uint24_t i, sum=0;
    for(i=0;i<ct;i++)
        sum+= arr[i];
    return sum;
}

void tick_ThisTick(sk_key_t* key){

    // PROCESS KEY INPUT
    *key = getKey();
    switch(*key){
        case sk_Clear:
            keybind_Clear();
            break;
        case sk_Vars:
            debug = !debug;
            break;
        case sk_Prgm:
            keybind_Prgm();
            break;
        case sk_Log:
            keybind_Log();
            break;
        case sk_Store:
            keybind_Store();
            break;
        case sk_Yequ:
            screen = (screen == SCRN_SENS) ? SCRN_OFF : SCRN_SENS;
            break;
        case sk_Window:
            screen = (screen == SCRN_TACT) ? SCRN_OFF : SCRN_TACT;
            break;
        case sk_Zoom:
            screen = (screen == SCRN_MAINS) ? SCRN_OFF : SCRN_MAINS;
            break;
        case sk_Trace:
            screen = (screen == SCRN_TRANSPORT) ? SCRN_OFF : SCRN_TRANSPORT;
            break;
        case sk_Graph:
            screen = (screen == SCRN_CARGO) ? SCRN_OFF : SCRN_CARGO;
            break;
        case sk_Stat:
            keybind_Stat();
            break;
        case sk_Mode:
            keybind_Mode();
            break;
        case sk_Right:
            keybind_RightArrow();
            break;
          case sk_Left:
            keybind_LeftArrow();
            break;
        case sk_Down:
            keybind_DownArrow();
            break;
        case sk_Up:
            keybind_UpArrow();
            break;
    }
    
    // RENDER BACKGROUND, MENUS, and LOG
    gfx_BlitScreen();
    Screen_RenderUI();
    gfx_RenderParticles(&particles, MAX_PARTICLES);
    if(arr_sum(log_display,4)) gui_ShowLog();
    // gfx_BlitBuffer();
    gfx_SwapDraw();
    ntwk_process();
    
    // PROCESS TICK COUNT, TIMINGS, TIMEOUTS
    /*if(netflags.logged_in){
        if(ticknum % 20 == 0) {
            ntwk_send(FRAMEDATA_REQUEST,
                PS_VAL(Ship.rot.yaw),
                PS_VAL(Ship.rot.pitch),
                PS_VAL(Ship.rot.roll));
             }
         }
         */
      if(ntwk_inactive_clock == settings.limits.network_timeout)  ntwk_send_nodata(PING);
      if(ntwk_inactive_clock >= ntwk_inactive_disconnect){
          gui_NetworkErrorResponse(1, 8, true);
          gameflags.loopgame = false;
          return;
      }
      ticknum++;
      ntwk_inactive_clock++;
}


uint8_t PlayGame(void){
    sk_key_t key = 0;
    uint24_t wait = 5000;
    full_redraw = true;
    if(gameflags.gfx_error) return GFX;
    if(!netflags.network_up) return NTWK;
    ntwk_inactive_disconnect = settings.limits.network_timeout * 11 / 10;
    gameflags.loopgame = true;
    netflags.bridge_error = false;
    gameflags.exit = false;
    netflags.bridge_up = false;
    gfx_InitModuleIcons();
    ntwk_send(CONNECT, PS_STR(settings.servers[bridge_config.server]));
    gfx_PrintStringXY("Waiting for bridge...", 20, 190);
    gfx_PrintStringXY("[Clear] to stop", 20, 200);
    gfx_BlitBuffer();
    do {
        key = getKey();
        ntwk_process();
        if(key==sk_Clear) return USER_RETURN;
        if(netflags.bridge_up==true) break;
        if(netflags.bridge_error==true) return BRIDGE;
        if(!wait--) return TIMEOUT;
    }   while(1);
    while(!netflags.client_version_ok){
        key = getKey();
        ntwk_process();
        if(key==sk_Clear) return USER_RETURN;
        if(gameflags.version_err) return VERSION;
    }
    wait = 5000;
    do {
        key = getKey();
        ntwk_process();
        if(netflags.logged_in) break;
        if(!wait--) return TIMEOUT;
    } while(1);
    gfx_ZeroScreen();
    if(key==sk_Clear) return USER_RETURN;
    while(!gameflags.exit){
        size_t bytes_read;
        tick_ThisTick(&key);
    }
    gfx_ClearParticles(&particles, MAX_PARTICLES);
    return NO_ERROR;
}
