#include <stdint.h>
#include "../../classes/ships.h"
#include "../../classes/systems/engines.h"
#include "../../gfx/TrekGFX.h"
#include "../screens.h"
#include "../gui.h"

char speedcfg_title[] = "[SPEED]";
char engnames[3][12] = {"sys.Thrust","sys.Engine","sys.Warp"};
char* tooltip = "[Sto] to engage";

uint8_t speed_PrintEngineInfo(uint8_t i, engine_data_t* eng, uint24_t x, uint8_t y){
    uint32_t curr_speed = eng->current_speed,
            max_speed = eng->max_speed,
            max_accel = eng->max_accel;
    uint8_t h=(select.engine == i) ? 50 : 20;
    char* units = (i==0) ? " m/s" : (i==1) ? " km/s" : " c";
    if(select.engine == i){
        if(max_speed){
            window_data_t win = {x, vWidth-90, y, h, 74, 2, 131};
            uint24_t speed_perc = curr_speed * 100 / max_speed;
            uint8_t barcolor = (curr_speed==0) ? 192 : 229;
            bar_colors_t colors = { barcolor, 0, 192 };
            uint24_t barw = (vWidth-90)/2;
            uint24_t indic_x = select.newspeed*barw/max_speed;
            gfx_RenderWindow(&win);
            gfx_PrintStringXY(engnames[i], x+3, y+5);
            stats_DrawHealthBar(speed_perc, barw, x+5, y+16, 15, &colors, false);
            gfx_FillRectangleColor(0, x+4+indic_x, y+15, 3, 15);
            gfx_SetTextXY(x+5, y+35);
            gfx_PrintUInt(select.newspeed, num_GetLength(select.newspeed));
            gfx_PrintString(units);
            x+=(barw+10);
            gfx_PrintStringXY("Max Speed:", x, y+7);
            gfx_SetTextXY(x+5, y+17);
            gfx_PrintUInt(max_speed, num_GetLength(max_speed));
            gfx_PrintString(units);
            gfx_PrintStringXY("Max Accel:", x, y+27);
            gfx_SetTextXY(x+5, y+37);
            gfx_PrintUInt(max_accel, num_GetLength(max_accel));
            gfx_PrintString(" unit/s");
        }
        else gfx_PrintStringXY("No module installed", x+2, y+15);
    }
    else {
        window_data_t win = {x, vWidth-90, y, h, 74, 2, 33};
        gfx_RenderWindow(&win);
        gfx_PrintStringXY(engnames[i], x+3, y+5);
    }
    return h;
}

void Screen_UISpeedConfig(engine_ref_t* eng_ref){
    uint24_t x = viewer_x + 30;
    uint8_t y = viewer_y, i;
    gfx_PrintStringXY(tooltip, x, y+115);
    gfx_RLETSprite(engspeed_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(speedcfg_title, viewer_x + 26, viewer_y - 1);
    y+=12;
    for(i=0; i<3; i++){
        uint8_t h = speed_PrintEngineInfo(i, &eng_ref->engine[i], x, y);
        y+=(h+2);
    }
}
