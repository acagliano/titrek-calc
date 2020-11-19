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

#include "rendering/screens.h"
#include "rendering/errors.h"
#include "rendering/gui.h"
#include "rendering/imgcaching.h"
#include "rendering/particles.h"

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
engine_ref_t engine_ref;

ti_var_t update_fp = 0;

/* Main Menu */

void PlayGame(void);
void ServerSelect(void);

void MainMenu(void) {
    uint8_t opt = 0;
    while(1){
        opt = gfx_RenderSplash(splash);
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
                if(key == sk_Down) sel = sel + (sel < (num_settings-1));
                if(key == sk_Up) sel = sel - (sel > 0);
                if(key == sk_Left) gfx_AlterSettingOpt(sel, -1);
                if(key == sk_Right) gfx_AlterSettingOpt(sel, 1);
                if(key || firstrender){
                    gfx_ZeroScreen();
                    for(i = 0; i < num_settings; i++)
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
                PlayGame();
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

    zx7_Decompress(err_icon, icon_error_compressed);
    zx7_Decompress(splash, splash_compressed);
    zx7_Decompress(icon_netup, icon_networkup_compressed);
    zx7_Decompress(icon_netdown, icon_networkdown_compressed);
    zx7_Decompress(log_error, log_error_compressed);
    zx7_Decompress(log_info, log_info_compressed);
    zx7_Decompress(log_debug, log_debug_compressed);
    zx7_Decompress(log_server, log_server_compressed);
    if(!ntwk_init()) goto error;
    
    gfx_GetVersion();
    gfx_VersionCheck();
    
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    strify_version(versionstr, version);
    do {
        MainMenu();
    } while(!gameflags.exit);
    if(!settings.savelogin) memset(&settings.userinfo, 0, sizeof(userinfo_t));
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
            if(netflags.logged_in){
                if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
                else ntwk_send_nodata(DISCONNECT);
            }
            else gameflags.loopgame = false;
            break;
        case sk_Vars:
            debug = !debug;
            break;
        case sk_Prgm:
            {
            char input[LOG_LINE_SIZE] = {0};
            prompt_for("[Chat Msg]", &input, LOG_LINE_SIZE-1, 5, 15, 0);
            if(input[0]) ntwk_send(MESSAGE, PS_STR(input));
            }
            break;
        case sk_Log:
            ntwk_send_nodata(GET_ENGINE_MAXIMUMS);
            screen = SCRN_NAVIG;
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
            if((screen == SCRN_MAINS) || (screen == SCRN_TACT)){
                uint8_t slot = (screen == SCRN_MAINS) ? select.mains : select.tactical;
                ntwk_send(MODULE_INFO_REQUEST, PS_VAL(slot));
                screen = setbits(screen, SCRN_INFO);
            }
            break;
        case sk_Mode:
            if((screen == SCRN_MAINS) || (screen == SCRN_TACT)){
                uint8_t slot = (screen == SCRN_MAINS) ? select.mains : select.tactical;
                const uint8_t action = CHANGE_ONLINE_STATE;
                ntwk_send(MODULE_STATE_CHANGE, PS_VAL(slot), PS_VAL(action));
            }
            break;
        case sk_Down:
        {
            char i;
            switch(screen){
                case SCRN_TACT:
                    for(i = select.tactical + 1; i < (MAX_MODULES - 1); i++){
                        int type = Ship.system[i].techclass;
                        if( type == mTactical ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case SCRN_MAINS:
                    for(i = select.mains + 1; i < (MAX_MODULES - 1); i++){
                        int type = Ship.system[i].techclass;
                        if( type == mSystem ){
                            select.mains = i;
                            break;
                        }
                    }
                    break;
                }
        }
            break;
        case sk_Up:
        {
            char i;
                  switch(screen){
                      case SCRN_TACT:
                          for(i = select.tactical - 1; i >= 0; i--){
                              int type = Ship.system[i].techclass;
                              if( type == mTactical ){
                                  select.tactical = i;
                                  break;
                              }
                          }
                          break;
                      case SCRN_MAINS:
                          for(i = select.mains - 1; i >= 0; i--){
                              int type = Ship.system[i].techclass;
                              if( type == mSystem ){
                                  select.mains = i;
                                  break;
                              }
                          }
                          break;
                  }

        }
            break;
    }
    
    // RENDER BACKGROUND, MENUS, and LOG
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


void PlayGame(void){
    sk_key_t key = 0;
    if(gameflags.gfx_error) return;
    if(!netflags.network_up) return;
    ntwk_inactive_disconnect = settings.limits.network_timeout * 11 / 10;
    gameflags.loopgame = true;
    netflags.bridge_error = false;
    if(!TrekGFX_init()) {
        dbg_sprintf(dbgout, "Failed to init graphics\n");
        return;
    }
    gfx_InitModuleIcons();
    ntwk_send(CONNECT, PS_VAL(settings.ssl_prefer), PS_STR(settings.servers[bridge_config.server]));
    gfx_PrintStringXY("Waiting for bridge...", 20, 190);
    gfx_PrintStringXY("[Clear] to stop", 20, 200);
    gfx_BlitBuffer();
    while(gameflags.loopgame){
        size_t bytes_read;
        key = getKey();
        if(netflags.bridge_error) break;
        if(netflags.bridge_up && netflags.client_version_ok){
            if(!gui_Login()){
                dbg_sprintf(dbgout, "Failed to login\n");
                break;
            }
        }
        while(netflags.bridge_up
            && netflags.network_up
            && gameflags.loopgame
            && netflags.client_version_ok){
            
            while(netflags.logged_in
                && netflags.network_up
                && netflags.bridge_up
                && gameflags.loopgame){
                tick_ThisTick(&key);
            }
            // log log out
            tick_ThisTick(&key);
        }
    // log disconnect
    tick_ThisTick(&key);
    }
    gfx_ClearParticles(&particles, MAX_PARTICLES);
}
