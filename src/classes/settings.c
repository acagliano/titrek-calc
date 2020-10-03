
#include "settings.h"
#include "player.h"
#include <string.h>
#include <stdbool.h>
#include <fileioc.h>

#define DEFAULT_NET_TIMEOUT 1200 // 60 seconds @ 20 tps

char settingtext[][20] = {
    "Save Login Info",
    "Prefer SSL",
    "Chunk Refresh Rate",
    "Entity Refresh Rate",
    "Packet Limit",
    "Ntwk Timeout Limit",
    "Log Display Length"
};

char settinginfo[][70] = {
    "Whether program should save login info.",
    "Always use SSL if available",
    "How often to request chunk data from server.",
    "How often to request entity data from server.",
    "Max number of packets to process per tick.",
    "Ticks w/o net activity triggering timeout.",
    "How long to display log lines for."
};

void set_defaults(void){
    // defaults are hardcoded within the game and cannot be changed unless through a rebuild
    // this may cause clients to be rejected by the server for
    // unauthorized modifications
    settings.limits.chunk_refresh = 1;
    settings.limits.entity_refresh = 1;
    settings.limits.packet_limit = 5;
    settings.limits.network_timeout = DEFAULT_NET_TIMEOUT;
    settings.limits.log_limit = 100;
    strcpy(settings.servers[0], "play.titrek.us");
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

gfx_RenderSettingOpt(uint8_t index, uint8_t selected, uint24_t x, uint8_t y){
    uint24_t val;
    gfx_SetTextFGColor(230);
    gfx_PrintStringXY(settingtext[index], x, y + 4);
    gfx_PrintStringXY(settinginfo[selected], 5, num_settings*16+45);
    gfx_SetColor(230);
    if(selected == index) gfx_FillCircle(x - 12, y + 7, 3);
    gfx_Rectangle(x + 150, y, 100, 12);
    gfx_SetColor(255);
    gfx_FillRectangle(x + 151, y + 1, 98, 10);
    gfx_SetTextFGColor(0);
    gfx_SetTextXY(x + 155, y + 3);
    switch(index){
        case SAVE_CREDS:
            val = settings.savelogin;
            if(val) gfx_PrintString("true");
            else gfx_PrintString("false");
            break;
        case SSL_PREFER:
            val = settings.ssl_prefer;
            if(val) gfx_PrintString("true");
            else gfx_PrintString("false");
            break;
        case CHUNK_REF:
            val = settings.limits.chunk_refresh;
            gfx_PrintUInt(val, num_GetLength(val));
            break;
        case ENTITY_REF:
            val = settings.limits.entity_refresh;
            gfx_PrintUInt(val, num_GetLength(val));
            break;
        case PACKET_LIMIT:
            val = settings.limits.packet_limit;
            gfx_PrintUInt(val, num_GetLength(val));
            break;
        case NTWK_TIMEOUT:
            val = settings.limits.network_timeout;
            gfx_PrintUInt(val, num_GetLength(val));
            break;
        case LOG_TIME:
            val = settings.limits.log_limit;
            gfx_PrintUInt(val, num_GetLength(val));
            break;
            
    }
}

void gfx_AlterSettingOpt(uint8_t selected, int8_t direction){
    switch(selected){
        case SAVE_CREDS:
            settings.savelogin = !settings.savelogin;
            break;
        case SSL_PREFER:
            settings.ssl_prefer = !settings.ssl_prefer;
            break;
        case CHUNK_REF:
            settings.limits.chunk_refresh += direction;
            break;
        case ENTITY_REF:
            settings.limits.entity_refresh += direction;
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
