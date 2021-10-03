#include <stdint.h>
#include <graphx.h>
#include <string.h>
#include "../../gfx/TrekGFX.h"
#include "../screens.h"
#include "../../classes/ships.h"

char mains_title[] = "[MAIN]";
#define CEMU_CONSOLE ((char*)0xFB0000)

void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected){
    gfx_RLETSprite(mains_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(mains_title, viewer_x + 26, viewer_y - 1);
    window_data_t win = {viewer_x + 40, vWidth - viewer_x - 60, viewer_y + 12, vHeight, 0, 0, 0};
    LCARS_RenderModuleList(systems, syscount, mSystem, &win, selected);
}
