#include <fileioc.h>

#include "settings.h"

const char *settingsvar = "TrekSave";

settings_t settings = {
    false,              // is debug mode meant to be enabled?
    {1, 1, 10},         // number of ticks between requests for new frame data
    {1, 1, 10},         // maximum number of packets to process per tick
    30000               // time to wait before triggering a network timeout, in milliseconds
};

bool settings_load(void){
    ti_var_t sfp = ti_Open(settingsvar, "r");
    if(!sfp) return false;
    if(ti_GetSize(sfp) != (sizeof settings)) return false;
    bool rv = ti_Read(&settings, sizeof settings, 1, sfp);
    ti_Close(sfp);
    return rv;
}

#define CEMU_CONSOLE ((char*)0xFB0000)
bool settings_write(void){
    ti_var_t sfp = ti_Open(settingsvar, "w");
    if(!sfp) return false;
    bool rv = ti_Write(&settings, sizeof settings, 1, sfp);
    ti_SetArchiveStatus(true, sfp);
    ti_Close(sfp);
    return rv;
}

enum _setting_opt {
    SETTING_DEBUG,
    SETTING_FRAMERATE,
    SETTING_PACKETLIMIT,
    SETTING_TIMEOUT
};

uint24_t setting_get(uint8_t opt){
    switch(opt){
        case SETTING_DEBUG:
            return settings.debug_mode;
        case SETTING_FRAMERATE:
            return settings.frame_refresh_rate.current;
        case SETTING_PACKETLIMIT:
            return settings.packet_limit.current;
        case SETTING_TIMEOUT:
            return settings.ntwk_timeout_limit;
        default:
            return 0xffffff;
    }
}
