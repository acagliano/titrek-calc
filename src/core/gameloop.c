#include <string.h>

#include <stdbool.h>
#include <fileioc.h>
#include <graphx.h>
#include <tice.h>

#include "../graphics/menus.h"
#include "../graphics/console.h"
#include "../gfx/uxassets.h"
#include "../asm/exposure.h"
#include "gameloop.h"
#include "controlcodes.h"
#include "network.h"
#include "ship.h"

game_error_t game_error = NO_ERROR;
tick_loop_mode_t tick_loop_mode = NO_CONNECTION;
hostinfo_t hostinfo = {0};
bool render_frame = false;
lcars_config_t lcars_config = {0, NULL};
bool render_console = false;

#define KF_OFFSET_HOSTNAME 7
void PlayGame(void){
    char keyfile[9] = {0};
    char *keyfile_start, *keyfile_hostname, *keyfile_username;
    ti_var_t kf;
    uint24_t timeout = 200000;
    
    if(!menu_SelectKeyFile(keyfile)) { game_error = KEY_NOT_FOUND; return; }
    if(!(kf = ti_Open(keyfile, "r"))) { game_error = KEY_IO_ERROR; return; }
    
    // save keyfile name, we need it to retrieve login token later
    strncpy(hostinfo.fname, keyfile, 8);
    
    // save hostname of server for convenience
    keyfile_start = ti_GetDataPtr(kf);    // return pointer to keyfile data
    ti_Close(kf);
    keyfile_hostname = keyfile_start + KF_OFFSET_HOSTNAME;
    keyfile_username = keyfile_hostname + strlen(keyfile_hostname) + 1;
    strncpy(hostinfo.hostname, keyfile_hostname, strlen(keyfile_hostname));
    strncpy(hostinfo.username, keyfile_username, strlen(keyfile_username));
    
    
    uint8_t out_ctl_code = CONNECT;
    
    #define CEMU_CONSOLE ((char*)0xFB0000)
    sprintf(CEMU_CONSOLE, "ctl: %u, file: %s\n", out_ctl_code, hostinfo.hostname);
    ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
    ntwk_queue(hostinfo.hostname, strlen(hostinfo.hostname));
    ntwk_send();
    
    render_console = true;

    do {
        sk_key_t key = getKey();
        
        // keybinds
        if(key == sk_Clear) break;
        // rendering switches for console/game ui
        if(render_console) console_render();
        else if(render_frame){
            ship_render_interior();
            ship_render_sysoverview(ship_systems);
        }
        ntwk_process();
    } while((tick_loop_mode > NO_CONNECTION) || timeout--);
}
