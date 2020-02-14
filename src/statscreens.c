#include <tice.h>
#include <string.h>
#include "classes/ships.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include "classes/screens.h"
#include <graphx.h>
#include <compression.h>
#include "gfx-engine/gui.h"
#include "classes/modules.h"
#include "gfx/moduleicons.h"
#include "equates.h"
#include <debug.h>
#define gfx_RectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_Rectangle((x1), (y1), (w), (h));
#define gfx_FillRectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_FillRectangle((x1), (y1), (w), (h));

char mains_title[] = "[MAIN SYS]";
char tact_title[] = "[TACTICAL SYS]";
char version[] = "v0.87 pre-alpha";

char moduledb[][12] = {
    "LifeSup",
    "Core",
    "WarpDr",
    "Impulse",
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

void Screen_RenderUI(uint24_t screen, ship_t* Ship, selected_t* select){
    Screen_Background(screen);
    gfx_SetTextFGColor(255);
    switch(screen & 0xff){
        case SCRN_OFF:
            break;
        case SCRN_SENS:
            break;
        case SCRN_TACT:
            Screen_UITacticalStats(&Ship->tactical, select->tactical);
            if(screen > 0xff)
                Overlay_UIModuleInfo(&Ship->tactical[select->tactical]);
            break;
        case SCRN_MAINS:
            Screen_UISystemStats(&Ship->system, select->mains);
            if(screen > 0xff)
                Overlay_UIModuleInfo(&Ship->system[select->mains]);
            break;
        case SCRN_TRANSPORT:
            break;
        case SCRN_CARGO:
            break;
    }
    gfx_BlitBuffer();
    gfx_SetTextFGColor(0);
    return;
}

void Screen_UISystemStats(module_t* systems, uint24_t selected){
    unsigned char i, cur_y;
    int cur_x;
    gfx_RLETSprite(mains_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(mains_title, viewer_x + 30, viewer_y + 1);
    for(i = 0; i < SYS_MAX; i++){
        // loop module health display
        module_t* module = &systems[i];
        cur_y = i * 18 + viewer_y + 14;
        cur_x = viewer_x + 40;
        if(selected == i) {
            gfx_SetColor(230);
            gfx_FillRectangle(cur_x - 6, cur_y, 6, 16);
        }
        gfx_SetColor(255);
        gfx_SetTextXY(cur_x, cur_y);
        module_RenderGeneral(module, cur_x, cur_y);
       
    }
    return;
}

void Screen_UITacticalStats(module_t* tactical, uint24_t selected){
    unsigned char i, cur_y = viewer_y + 20;
    int cur_x = viewer_x;
    uint24_t shield_health = 0, shield_resist = 0, shield_num = 0;
    bool shields_active = false;
    gfx_RLETSprite(tactical_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(tact_title, viewer_x + 37, viewer_y + 1);
    for(i = 0; i < TACT_MAX; i++){
        int temp_y = 18 * i + viewer_y + 14;
        int temp_x = cur_x + 90;
        module_t* module = &tactical[i];
        if(selected == i) {
            gfx_SetColor(230);
            gfx_FillRectangle(temp_x - 6, temp_y, 6, 16);
        }
        module_RenderGeneral(module, temp_x, temp_y);
        if(module->techtype == SHIELD){
            if(module->online){
                shields_active = true;
                shield_health += health_GetHealthPercent(&module->health);
                shield_resist *= module->data.mod_shields.resistance;
                shield_num++;
            }
        }
    }
        if(shields_active) {
            gfx_rletsprite_t* shield_icon = icon_shieldsstable;
            shield_health /= shield_num;
            if(shield_health <= 50) shield_icon = icon_shieldscollapse;
            if(shield_health <= 25) shield_icon = icon_shieldsfail;
            gfx_RLETSprite(shield_icon, cur_x + 5, cur_y + 5);
            cur_y += shield_icon->height + 10;
            gfx_RLETSprite(icon_shieldinteg, cur_x, cur_y - 1);
            cur_x += 12;
            stats_DrawHealthBar(shield_health, 60, cur_x, cur_y, 25, 107, 29);
            cur_y += 15; cur_x = viewer_x;
        }
        gfx_RLETSprite(icon_hullinteg, cur_x, cur_y - 1);
        cur_x += 12;
        stats_DrawHealthBar(health_GetHealthPercent(&tactical[TACT_MAX].health), 60, cur_x, cur_y, 33, 107, 74);
}





void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y){
    int24_t health = health_GetHealthPercent(&module->health);
    uint8_t bar = (module->techtype < SYS_MAX) ? 77 : 29;
    uint24_t x_space = vWidth - (x + 10);
    uint24_t width = (x_space > 200) ? 200 : x_space;
    gfx_RectangleColor(229, x, y, width, 16);
    if(module->assigned){
        uint8_t techtype = module->techtype;
        uint24_t barwidth;
        x_space -= 90;
        barwidth = (x_space > 100) ? 100 : x_space;
        if(module->online){
            gfx_FillRectangleColor(3, x + 1, y + 1, width - 2, 14);}
        else{
            gfx_FillRectangleColor(96, x + 1, y + 1, width - 2, 14);}
        gfx_FillRectangleColor(0, x+1, y+1, 14, 14);
        gfx_RLETSprite(modicons[techtype], x + 1, y + 1);
        gfx_PrintStringXY(moduledb[techtype], x + 20, y + 5);
        stats_DrawHealthBar(health, barwidth, x + 80, y + 4, 0, 149, bar);
    }
    else {
        gfx_FillRectangleColor(74, x + 1, y + 1, width - 2, 14);
        gfx_PrintStringXY("unassigned", x + 10, y + 5);
    }
}


void Overlay_UIModuleInfo(module_t* module) {
    int24_t health = health_GetHealthPercent(&module->health),
            power = power_GetBatteryPercent(&module->power);
    uint24_t x = 60, width = 200;
    uint8_t y = 30, height = 110;
    uint24_t text_x = x + 4;
    uint8_t text_y = y + 4;
    uint24_t techtype = module->techtype;
    gfx_SetTextFGColor(0);
    gfx_RenderWindow(x, y, width, height, 139, 205, 3);
    gfx_FillRectangleColor(139, x + 2, y + 2, width - 4, 16);
    gfx_RLETSprite(modicons[techtype], text_x + 2, y + 2);
    gfx_PrintStringXY(modulenames[techtype], text_x + 20, text_y + 2);
    text_y += 17;
    gfx_RLETSprite(icon_health, text_x, text_y - 2);
    stats_DrawHealthBar(health, 130, text_x + 15, text_y, 0, 149, 77);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(health, 1 + (health > 9) + (health > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_RLETSprite(icon_sourcereserve, text_x, text_y - 2);
    stats_DrawHealthBar(power, 130, text_x + 15, text_y, 0, 149, 77);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(power, 1 + (power > 9) + (power > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_PrintStringXY("Status: ", text_x + 2, text_y);
    if(module->online) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
}








void Screen_Background(unsigned char active) {
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
    gfx_PrintStringXY(version, 160 - (7 * strlen(version) / 2), 0);
    gfx_SetTextFGColor(0);
}
