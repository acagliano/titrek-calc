#include <string.h>

#include <stdbool.h>
#include <fileioc.h>

#include "../graphics/menus.h"
#include "gameloop.h"
#include "network.h"

game_error_t game_error = NO_ERROR;
tick_loop_mode_t tick_loop_mode = NO_CONNECTION;
hostinfo_t hostinfo = {0};

#define KF_OFFSET_HOSTNAME 7
void PlayGame(void){
    char keyfile[9] = {0};
    char *keyfile_start, *keyfile_hostname, *keyfile_token;
    size_t keyfile_len;
    ti_var_t kf;
    
    if(!menu_SelectKeyFile(keyfile)) { game_error = KEY_NOT_FOUND; return; }
    if(!(kf = ti_Open(keyfile, "r"))) { game_error = KEY_IO_ERROR; return; }
    
    // save keyfile name, we need it to retrieve login token later
    strncpy(hostinfo.fname, keyfile, 8);
    
    // save hostname of server for convenience
    keyfile_start = ti_GetDataPtr(kf);    // return pointer to keyfile data
    keyfile_hostname = keyfile_start + KF_OFFSET_HOSTNAME;
    strncpy(hostinfo.hostname, keyfile_hostname, strlen(keyfile_hostname));
    
    
    
}
