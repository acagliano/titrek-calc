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
#include <debug.h>

char mains_title[] = "[MAIN SYS CONTROL]";
char shields_title[] = "[DEFENSE CONTROL]";
char tact_title[] = "[TACTICAL SYS CONTROL]";
char version[] = "v0.84 pre-alpha";

char moduledb[][15] = {
    "LifeSupp",
    "WarpCore",
    "ImpulseDr",
    "NavSensor",
    "Transport",
    "Shields",
    "Armor",
    "Phaser",
    "Torpedo",
    "TargSensor"
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
            break;
        case SCRN_MAINS:
            Screen_UISystemStats(&Ship->system, select->mains);
            break;
        case SCRN_TRANSPORT:
            break;
        case SCRN_CARGO:
            break;
    }
    if(screen > 0xff) {
        gfx_PrintStringXY("Info Window Here", 60, 100);
    }
    gfx_BlitBuffer();
    gfx_SetTextFGColor(0);
    return;
}

void Screen_UISystemStats(module_t* systems, uint24_t selected){
    unsigned char i, cur_y;
    int cur_x;
    gfx_PrintStringXY(mains_title, text_GetCenterX(mains_title), viewer_y);
    for(i = 0; i < SYS_MAX; i++){
        // loop module health display
        module_t* module = &systems[i];
        cur_y = i * 15 + viewer_y + 17;
        cur_x = viewer_x;
        if(selected == i) {
            gfx_SetColor(231);
            gfx_FillRectangle(cur_x - 6, cur_y - 3, 5, 13);
        }
        gfx_SetColor(255);
        gfx_SetTextXY(cur_x, cur_y);
        if(module->assigned){
            uint8_t techtype = module->techtype;
            gfx_rletsprite_t *icon = ((gfx_rletsprite_t*)trekgui[MODICONS + techtype]);
            dbg_sprintf(dbgout, "%u\n", techtype);
            dbg_sprintf(dbgout, "%u\n", MODICONS);
            dbg_sprintf(dbgout, "%u\n", icon);
            dbg_Debugger();

            gfx_RLETSprite(icon, cur_x, cur_y);
            gfx_PrintStringXY(moduledb[techtype], cur_x + 20, cur_y);
        }
    }
    return;
}

void Screen_UITacticalStats(module_t* tactical, uint24_t selected){
    unsigned char i, cur_y = viewer_y + 20;
    int cur_x = viewer_x;
    uint24_t shield_health = 0, shield_resist = 0, shield_num = 0;
    bool shields_active = false;
    gfx_PrintStringXY(shields_title, text_GetCenterX(shields_title), viewer_y);
    for(i = 0; i < TACT_MAX; i++){
        int temp_y = 22 * i + viewer_y + 20;
        int temp_x = cur_x + 110;
        module_t* module = &tactical[i];
        if(module->assigned){
            if(module->techtype == SHIELD){
                if(module->online){
                    shields_active = true;
                    shield_health += health_GetHealthPercent(&module->health);
                    shield_resist *= module->data.mod_shields.resistance;
                    shield_num++;
                }
            }
            if(selected == i) {
                gfx_SetColor(230);
                gfx_FillRectangle(temp_x, temp_y, 10, 20);
            }
            temp_x += 10;
            gfx_SetColor(31);
            if(!module->online) gfx_SetColor(160);
            gfx_FillRectangle(temp_x, temp_y, 140, 20);
            gfx_SetColor(33);
            gfx_FillRectangle(temp_x + 2, temp_y + 2, 136, 16);
            temp_x += 5; temp_y += 6;
            stats_DrawHealthBar(health_GetHealthPercent(&module->health), 80, temp_x, temp_y, 255, 255);
            }
        }
        if(shields_active) {
            gfx_rletsprite_t* shield_icon = icon_shieldsstable;
            shield_health /= shield_num;
            if(shield_health <= 50) shield_icon = icon_shieldscollapse;
            if(shield_health <= 25) shield_icon = icon_shieldsfail;
            gfx_RLETSprite(shield_icon, cur_x, cur_y);
            cur_y += shield_icon->height + 10;
            stats_DrawHealthBar(shield_health, 75, cur_x, cur_y, 30, 255);
            cur_y += 15; cur_x = viewer_x;
        }
        stats_DrawHealthBar(health_GetHealthPercent(&tactical[TACT_MAX].health), 75, cur_x, cur_y, 255, 255);
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
    gfx_PrintStringXY("Defense", 4, lcars_texty);
    gfx_PrintStringXY("Tact", 80, lcars_texty);
    gfx_PrintStringXY("Mains", 140, lcars_texty);
    gfx_PrintStringXY("Repair", 200, lcars_texty);
    gfx_PrintStringXY("Cargo", 268, lcars_texty);
    gfx_SetTextFGColor(148);
    gfx_PrintStringXY(version, 160 - (7 * strlen(version) / 2), 0);
    gfx_SetTextFGColor(0);
}
