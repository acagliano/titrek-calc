#include <string.h>
#include "../../gfx/TrekGFX.h"
#include "../../versioning.h"
#include "../screens.h"
#include "../gui.h"
#include "../colors.h"

char moduledb[][12] = {
    "LifeSup",
    "Core",
    "Thrust",
    "Engine",
    "WarpDr",
    "NavSens",
    "Transpt",
    "Shields",
    "Armor",
    "Phasers",
    "Torpedo",
    "TargSens"
};
char modulenames[][20] = {
    "Life Support",
    "Warp Core",
    "Warp Drive",
    "Sublight Engines",
    "Nav Sensors",
    "Transporter",
    "Shields",
    "Hull Armor",
    "Phasers",
    "Torpedo",
    "Targetting Sensors"
};

void LCARS_RenderModuleList(module_t* systems, uint24_t syscount, uint8_t class, window_data_t* w, uint24_t selected){
    uint24_t x = w->x, width = w->w;
    uint8_t y = w->y, height = w->h;
    uint8_t i;
    for(i = 0; i<syscount; i++){
        module_t* module = &systems[i];
        if(module->techclass == class){
            if(selected == i) {
                gfx_SetColor(230);
                gfx_FillRectangle(x - 6, y, 6, 14);
            }
            gfx_SetColor(255);
            gfx_SetTextXY(x, y);
            module_RenderGeneral(module, x, y, width);
            y += 16;
        }
    }
}

void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y, uint24_t width){
    int24_t health = module->health;
    uint8_t bar = (module->techtype < SYS_MAX) ? SYS_HEALTHBAR_COLOR : TACT_HEALTHBAR_COLOR;
    bar_colors_t healthbar = {bar, ALL_HEALTHBAR_BORDER, ALL_HEALTHBAR_BG};
    gfx_RectangleColor(MODULE_BORDER, x, y, width, 14);
    if(module->techclass){
        uint8_t techtype = module->techtype;
        uint24_t barwidth = (width > 200) ? 100 : (width * 3.5 / 10);
        uint8_t color = (module_OnlineState(module)) ? BG_MODULE_ONLINE : BG_MODULE_OFFLINE;
        gfx_FillRectangleColor(color, x + 1, y + 1, width - 2, 12);
        gfx_FillRectangleColor(TRANSPARENT_COLOR, x+1, y+1, 12, 12);
        gfx_RLETSprite(modicons[techtype], x + 1, y + 1);
        gfx_PrintStringXY(module->name, x + 15, y + 4);
        stats_DrawHealthBar(health, barwidth, x + 80, y + 3, 10, &healthbar, true);
        stats_RenderPowerIndic(module_PowerWarn(module), x + 80 + barwidth + 5, y);
    }
    else {
        gfx_FillRectangleColor(74, x + 1, y + 1, width - 2, 14);
        gfx_PrintStringXY("unassigned", x + 10, y + 5);
    }
}

void Screen_RenderUI(void){
    Screen_Background(screen);
    gfx_SetTextFGColor(255);
    switch(screen & 0xff){
        case SCRN_OFF:
            break;
        case SCRN_SENS:
            break;
        case SCRN_TACT:
            Screen_UITacticalStats(Ship.hull, Ship.system, MAX_MODULES, select.tactical);
            if(screen > 0xff)
                Overlay_UIModuleInfo();
            break;
        case SCRN_MAINS:
            Screen_UISystemStats(Ship.system, MAX_MODULES, select.mains);
            if(screen > 0xff)
                Overlay_UIModuleInfo();
            break;
        case SCRN_TRANSPORT:
            break;
        case SCRN_CARGO:
            break;
        case SCRN_NAVIG:
            Screen_UISpeedConfig(&engine_ref);
            break;
    }
    gfx_SetTextFGColor(0);
    return;
}

void Screen_Background(uint8_t active) {
    int i;
    gfx_SetTextFGColor(255);
    gfx_ZeroScreen();
    gfx_RLETSprite(shipinterior_left, 0, 0);
    gfx_RLETSprite(shipinterior_right, 160, 0);
    for(i = 0; i < 320; i++){
        gfx_SetColor(0);
        gfx_Rectangle(i * 64, 216, 64, 20);
    }
    gfx_SetColor(116);
    if(active) gfx_FillRectangle(--active*64+1, 217, 62, 18);
    gfx_PrintStringXY("Sensors", 4, lcars_texty);
    gfx_PrintStringXY("Tactical", 68, lcars_texty);
    gfx_PrintStringXY("MainSys", 134, lcars_texty);
    gfx_PrintStringXY("Repair", 200, lcars_texty);
    gfx_PrintStringXY("Cargo", 268, lcars_texty);
    gfx_SetTextFGColor(148);
    gfx_PrintStringXY(versionstr, 160 - (7 * strlen(versionstr) / 2), 1);
    gfx_SetTextFGColor(0);
}
