#include <stdint.h>
#include <graphx.h>
#include "../../gfx/TrekGFX.h"
#include "../screens.h"
#include "../colors.h"
#include "../gui.h"
#include "../../classes/modules.h"

char tact_title[] = "[TACT]";

void Screen_UITacticalStats(hull_t hull, module_t* systems, uint24_t syscount, uint24_t selected){
    unsigned char i, j=0, cur_y = viewer_y + 30;
    int cur_x = viewer_x;
    uint24_t shield_health = 0, shield_resist = 0, shield_num = 0;
    bool shields_active = false;
    bar_colors_t hullbar = {HULLINTEGBAR_COLOR, HULLINTEGBAR_BORDER, HULLINTEGBAR_BG};
    bar_colors_t shieldbar = {SHIELDINTEGBAR_COLOR, SHIELDINTEGBAR_BORDER, SHIELDINTEGBAR_BG};
    gfx_RLETSprite(tactical_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(tact_title, viewer_x + 26, viewer_y - 1);
    window_data_t win = {viewer_x + 90, vWidth - viewer_x - 100, viewer_y + 14, vHeight};
    LCARS_RenderModuleList(systems, syscount, mTactical, &win, selected);
    for(i = 0; i < syscount; i++){
        module_t* module = &systems[i];
        if(module->techclass == mTactical){
            if(module->techtype == SHIELD){
                if(module_OnlineState(module)){
                    shields_active = true;
                    shield_health += module->health;
                    shield_num++;
                }
            }
        }
    }
    shield_health /= shield_num;
    gfx_DrawShieldDisp(shields_active, shield_health, top, cur_x, cur_y - 1);
    gfx_DrawShieldDisp(shields_active, shield_health, side, cur_x + 40, cur_y - 1);
    cur_y += 42;
    gfx_RLETSprite(icon_shieldinteg, cur_x, cur_y - 1);
    cur_x += 12;
    if(shield_num) stats_DrawHealthBar(shield_health, 60, cur_x, cur_y, 10, &shieldbar);
    else gfx_PrintStringXY("None", cur_x + 2, cur_y+2);
    cur_y += 16;
    cur_x = viewer_x;
    gfx_RLETSprite(icon_hullinteg, cur_x, cur_y - 1);
    cur_x += 12;
    stats_DrawHealthBar(hull.health, 60, cur_x, cur_y, 10, &hullbar);
}
