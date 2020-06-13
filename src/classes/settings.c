
#include "settings.h"
#include <stdbool.h>
#include <fileioc.h>

void set_defaults(void){
    // defaults are hardcoded within the game and cannot be changed unless through a rebuild
    // this may cause clients to be rejected by the server for
    // unauthorized modifications
    settings.limits.chunk_refresh = 1;
    settings.limits.entity_refresh = 1;
    settings.limits.packet_limit = 5;
}

bool write_settings(void){
    ti_var_t savefile = ti_Open(settingsappv, "r+");
    if(!savefile) return false;
    ti_Write(&settings, sizeof(settings), 1, savefile);
    ti_SetArchiveStatus(true, savefile);
    ti_Close(savefile);
    return true;
}
