
#include <string.h>
#include <stdbool.h>
#include <fileioc.h>
#include <graphx.h>
#include <stdint.h>
#include "settings.h"
#include "player.h"
#include "../lcars/screens.h"

#define DEFAULT_NET_TIMEOUT 1200 // 60 seconds @ 20 tps

char settingtext[][20] = {
    "Debug Mode",
    "Frame Refresh Rate",
    "Packet Limit",
    "Ntwk Timeout Limit",
    "Log Display Length"
};

char settinginfo[][70] = {
    "Whether program should log debug msgs.",
    "How often to request frame data from server.",
    "Max number of packets to process per tick.",
    "Ticks w/o net activity triggering timeout.",
    "How long to display log lines for."
};

enum _accessmode {
    SETT_BOOL,
    SETT_CHAR,
    SETT_INT
};


void set_defaults(void){
    // defaults are hardcoded within the game and cannot be changed unless through a rebuild
    // this may cause clients to be rejected by the server for
    // unauthorized modifications
    settings.limits.chunk_refresh = 1;
    settings.limits.packet_limit = 5;
    settings.limits.network_timeout = DEFAULT_NET_TIMEOUT;
    settings.limits.log_limit = 100;
}

bool write_settings(void){
    ti_var_t savefile = ti_Open(settingsappv, "r+");
    if(!savefile)
        if(!(savefile = ti_Open(settingsappv, "w+"))) return false;
    ti_Write(&settings, sizeof(settings), 1, savefile);
    ti_SetArchiveStatus(true, savefile);
    ti_Close(savefile);
    return true;
}

void gfx_SettingOptRender(void* val, uint8_t mode){
    switch(mode){
        case SETT_BOOL:
        {
            bool n = *(bool*)val;
            if(n) gfx_PrintString("true");
            else gfx_PrintString("false");
        }
            break;
        case SETT_CHAR:
        {
            uint8_t n = *(uint8_t*)val;
            gfx_PrintUInt(n, num_GetLength(n));
        }
            break;
        case SETT_INT:
        {
            uint24_t n = *(uint24_t*)val;
            gfx_PrintUInt(n, num_GetLength(n));
        }
            break;
    }
}

void gfx_RenderSettingOpt(uint8_t index, uint8_t selected, uint24_t x, uint8_t y){
    gfx_SetTextFGColor(230);
    gfx_PrintStringXY(settingtext[index], x, y + 4);
    gfx_PrintStringXY(settinginfo[selected], 5, num_settings*16+30);
    gfx_SetColor(230);
    if(selected == index) gfx_FillCircle(x - 12, y + 7, 3);
    gfx_Rectangle(x + 150, y, 100, 14);
    gfx_SetColor(255);
    gfx_FillRectangle(x + 151, y + 1, 98, 12);
    gfx_SetTextFGColor(0);
    gfx_SetTextXY(x + 155, y + 4);
    switch(index){
        case DEBUG_MODE:
            gfx_SettingOptRender(&settings.debug, SETT_BOOL);
            break;
        case CHUNK_REF:
            gfx_SettingOptRender(&settings.limits.chunk_refresh, SETT_CHAR);
            break;
        case PACKET_LIMIT:
            gfx_SettingOptRender(&settings.limits.packet_limit, SETT_CHAR);
            break;
        case NTWK_TIMEOUT:
            gfx_SettingOptRender(&settings.limits.network_timeout, SETT_INT);
            break;
        case LOG_TIME:
            gfx_SettingOptRender(&settings.limits.log_limit, SETT_INT);
            break;
            
    }
}

void gfx_AlterSettingOpt(uint8_t selected, int8_t direction){
    switch(selected){
        case DEBUG_MODE:
            settings.debug = !settings.debug;
            break;
        case CHUNK_REF:
            settings.limits.chunk_refresh += direction;
            break;
        case PACKET_LIMIT:
            settings.limits.packet_limit += direction;
            break;
        case NTWK_TIMEOUT:
            settings.limits.network_timeout += (direction * 20);
            break;
        case LOG_TIME:
            settings.limits.log_limit += (direction * 10);
            break;
    }

}
