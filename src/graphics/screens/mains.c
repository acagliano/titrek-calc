#include <stdint.h>
#include <graphx.h>
#include "../../gfx/TrekGFX.h"
#include "../screens.h"
#include "../../classes/ships.h"

char mains_title[] = "[MAIN]";

void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected){
    gfx_RLETSprite(mains_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(mains_title, viewer_x + 26, viewer_y - 1);
    window_data_t win = {viewer_x + 40, vWidth - viewer_x - 60, viewer_y + 12, vHeight};
    LCARS_RenderModuleList(systems, syscount, mSystem, &win, selected);
}
