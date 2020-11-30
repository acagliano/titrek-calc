#include <stdint.h>
#include <graphx.h>
#include "../../gfx/TrekGFX.h"
#include "../screens.h"
#include "../colors.h"
#include "../gui.h"
#include "../../classes/modules.h"

void Overlay_UIModuleInfo(void) {
    bar_colors_t healthbar = {SYS_HEALTHBAR_COLOR, ALL_HEALTHBAR_BORDER, ALL_HEALTHBAR_BG};
    uint24_t x = 60, width = 200;
    uint8_t y = 30, height = 110;
    uint24_t text_x = x + 4;
    uint8_t text_y = y + 4;
    window_data_t win = {x, width, y, height, 139, 3, 205};
    gfx_SetTextFGColor(0);
    gfx_RenderWindow(&win);
    gfx_FillRectangleColor(139, x + 2, y + 2, width - 4, 16);
    if(!ModuleInfo.modclass) {
        gfx_PrintStringXY("No Module Info Loaded", text_x + 20, text_y + 2);
        return;
    }
    uint8_t state = ModuleInfo.state;
    uint8_t techtype = ModuleInfo.modtype;
    uint8_t health = health_GetPercent(&ModuleInfo.health);
    uint8_t power = power_GetReservePercent(&ModuleInfo.power);
    
    gfx_RLETSprite(modicons[techtype], text_x + 2, y + 2);
    gfx_PrintStringXY(modulenames[techtype], text_x + 20, text_y + 2);
    text_y += 17;
    gfx_RLETSprite(icon_health, text_x, text_y - 2);
    stats_DrawHealthBar(health, 130, text_x + 15, text_y, 10, &healthbar, true);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(health, 1 + (health > 9) + (health > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_RLETSprite(icon_sourcereserve, text_x, text_y - 2);
    stats_DrawHealthBar(power, 130, text_x + 15, text_y, 10, &healthbar, true);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(power, 1 + (power > 9) + (power > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_PrintStringXY("Status: ", text_x + 2, text_y);
    if(state) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
}
