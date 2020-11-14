
#include <tice.h>
#include <string.h>
#include <graphx.h>
#include <compression.h>
#include <debug.h>
#include "../classes/tech.h"
#include "../classes/ships.h"
#include "../classes/modules.h"

#include "../gfx/moduleicons.h"
#include "../gfx/TrekGFX.h"
#include "gui.h"
#include "colors.h"

#include "../equates.h"
#include "screens.h"

#define gfx_RectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_Rectangle((x1), (y1), (w), (h));
#define gfx_FillRectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_FillRectangle((x1), (y1), (w), (h));

char mains_title[] = "[MAIN]";
char tact_title[] = "[TACT]";

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

int text_GetCenterX(char* string){
    return (LCD_WIDTH - gfx_GetStringWidth(string)) / 2;
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100);
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
            break;
    }
    gfx_SetTextFGColor(0);
    return;
}

void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected){
    gfx_RLETSprite(mains_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(mains_title, viewer_x + 26, viewer_y - 1);
    window_data_t win = {viewer_x + 40, vWidth - viewer_x - 60, viewer_y + 12, vHeight};
    LCARS_RenderModuleList(systems, syscount, mSystem, &win, selected);
}

void LCARS_RenderModuleList(module_t* systems, uint24_t syscount, uint8_t class, window_data_t* w, uint24_t selected){
    uint24_t x = w->x, width = w->width;
    uint8_t y = w->y, height = w->height;
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

void Screen_UITacticalStats(hull_t hull, module_t* systems, uint24_t syscount, uint24_t selected){
    unsigned char i, j=0, cur_y = viewer_y + 30;
    int cur_x = viewer_x;
    uint24_t shield_health = 0, shield_resist = 0, shield_num = 0;
    bool shields_active = false;
    bar_colors_t hullbar = {HULLINTEGBAR_COLOR, HULLINTEG_BORDER, HULLINTEG_BG};
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
    gfx_RLETSprite(icon_hullinteg, cur_x, cur_y - 1);
    cur_x += 12;
    stats_DrawHealthBar(hull.health, 60, cur_x, cur_y, 10, &hullbar);
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
        gfx_PrintStringXY(moduledb[techtype], x + 15, y + 4);
        stats_DrawHealthBar(health, barwidth, x + 80, y + 3, 10, &healthbar);
        stats_DrawPowerBar(module_PowerWarn(module), x + 80 + barwidth + 5, y + 2);
    }
    else {
        gfx_FillRectangleColor(74, x + 1, y + 1, width - 2, 14);
        gfx_PrintStringXY("unassigned", x + 10, y + 5);
    }
}

void Overlay_UIModuleInfo(void) {
    bar_colors_t healthbar = {SYS_HEALTHBAR_COLOR, ALL_HEALTHBAR_BORDER, ALL_HEALTHBAR_BG};
    uint24_t x = 60, width = 200;
    uint8_t y = 30, height = 110;
    uint24_t text_x = x + 4;
    uint8_t text_y = y + 4;
    gfx_SetTextFGColor(0);
    gfx_RenderWindow(x, y, width, height, 139, 205, 3);
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
    stats_DrawHealthBar(health, 130, text_x + 15, text_y, 10, &healthbar);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(health, 1 + (health > 9) + (health > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_RLETSprite(icon_sourcereserve, text_x, text_y - 2);
    stats_DrawHealthBar(power, 130, text_x + 15, text_y, 10, &healthbar);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(power, 1 + (power > 9) + (power > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_PrintStringXY("Status: ", text_x + 2, text_y);
    if(state) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
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


