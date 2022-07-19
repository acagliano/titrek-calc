#include <string.h>

#include <stdbool.h>
#include <fileioc.h>
#include <graphx.h>
#include <tice.h>

#include "../graphics/menus.h"
#include "../graphics/console.h"
#include "../gfx/TrekGFX.h"
#include "../asm/exposure.h"
#include "gameloop.h"
#include "network.h"

game_error_t game_error = NO_ERROR;
tick_loop_mode_t tick_loop_mode = NO_CONNECTION;
hostinfo_t hostinfo = {0};
bool render_frame = false;
lcars_config_t lcars_config = {0, NULL};

#define KF_OFFSET_HOSTNAME 7
void PlayGame(void){
    char keyfile[9] = {0};
    char *keyfile_start, *keyfile_hostname;
    ti_var_t kf;
    bool console_up = false;
    
    if(!menu_SelectKeyFile(keyfile)) { game_error = KEY_NOT_FOUND; return; }
    if(!(kf = ti_Open(keyfile, "r"))) { game_error = KEY_IO_ERROR; return; }
    
    // save keyfile name, we need it to retrieve login token later
    strncpy(hostinfo.fname, keyfile, 8);
    
    // save hostname of server for convenience
    keyfile_start = ti_GetDataPtr(kf);    // return pointer to keyfile data
    ti_Close(kf);
    keyfile_hostname = keyfile_start + KF_OFFSET_HOSTNAME;
    strncpy(hostinfo.hostname, keyfile_hostname, strlen(keyfile_hostname));
    
    do {
        sk_key_t key = getKey();
        
        // keybinds
        if(key == sk_Clear) break;
        // rendering switches for console/game ui
        if(console_up) console_render();
        else if(render_frame){
            gfx_RLETSprite(shipinterior_left, 0, 0);
            gfx_RLETSprite(shipinterior_right, (320>>1), 0);
        }
    } while(tick_loop_mode > NO_CONNECTION);
}
