#include <tice.h>
#include <string.h>
#include "classes/ships.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include <graphx.h>
#include <compression.h>

char mains_title[] = "[MAIN SYS CONTROL]";
char shields_title[] = "[SHIELDS CONTROL]";
char tact_title[] = "[TACTICAL SYS CONTROL]";
char version[] = "v0.83 pre-alpha";

char mainsys_strings[][10] = {"Integrity", "LifeSupp", "WarpCore", "WarpDrive", "Impulse", "NavSens", "Transport"};

int text_GetCenterX(char* string){
    return (LCD_WIDTH - gfx_GetStringWidth(string)) / 2;
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100);
}

void Screen_UISystemStats(module_t* system, unsigned char count, unsigned char selected){
    unsigned char i, cur_y;
    int cur_x;
    gfx_PrintStringXY(mains_title, text_GetCenterX(mains_title), viewer_y);
    for(i = 0; i < count; i++){
        // loop module health display
        module_t* module = &system[i];
        cur_y = i * 15 + viewer_y + 17;
        cur_x = viewer_x;
        if(selected == i) {
            gfx_SetColor(231);
            gfx_FillRectangle(cur_x - 6, cur_y - 3, 5, 13);
        }
        gfx_SetColor(255);
        gfx_SetTextXY(cur_x, cur_y);
        if(module->techclass){
            signed int health = health_GetHealthPercent(&module->health),
                        spend = power_GetPowerSpend(&module->power),
                        spendpercent = power_GetSpendPercent(&module->power),
                        source = power_GetDrawSource(&module->power),
                        pow_res = power_GetBatteryPercent(&module->power);
            gfx_PrintString(mainsys_strings[module->techtype]);
            cur_x += 75;
            cur_x = Stats_DrawHealthBar(health, 50, cur_x, cur_y, 255, 255, true);
            cur_x += 5;
            if(module->online) {
                gfx_RLETSprite((gfx_rletsprite_t*)trekgui[9 + source], cur_x, cur_y - 2);
                if(source < 2){
                    unsigned int draw = power_GetPowerDraw(&module->power);
                    cur_x += 11;
                    gfx_RLETSprite(icon_powerdraw, cur_x, cur_y - 2);
                    cur_x += 13;
                    gfx_Rectangle(cur_x, cur_y - 2, 25, 11);
                    gfx_SetTextXY(cur_x + 5, cur_y);
                    gfx_PrintUInt(draw, num_GetLength(draw));
                    cur_x += 30;
                    cur_x = Stats_DrawHealthBar(pow_res, 20, cur_x, cur_y, 255, 55, false);
                    gfx_FillRectangle(cur_x + 2, cur_y, 2, 7);
                    cur_x += 5;
                }
                else {
                    
                }
                gfx_RLETSprite(icon_powerspend, cur_x, cur_y - 2);
                cur_x += 13;
                gfx_Rectangle(cur_x, cur_y - 2, 25, 11);
                gfx_SetTextXY(cur_x + 5, cur_y);
                gfx_PrintUInt(spend, num_GetLength(spend));
            }
            else {
                gfx_RLETSprite(icon_offline, cur_x, cur_y - 2);
                gfx_PrintStringXY("offline", cur_x + 15, cur_y);
            }
            
            //gfx_SetTextXY(viewer_x + 100, (i + 1) * 10 + viewer_y + 5);
            //gfx_PrintUInt(health, 3);
        }
    }
    return;
}

void Screen_UIShieldStats(module_t* shields){
    unsigned char i, cur_y = viewer_y + 20;
    int cur_x = viewer_x;
    int shields_combined = 0, shields_active = 0;
    gfx_PrintStringXY(shields_title, text_GetCenterX(shields_title), viewer_y);
    for(i = 0; i < SHIELD_MAX; i++){
        module_t* shield = &shields[i];
        int resist = shield->data.shields.resistance;
        int health = health_GetHealthPercent(&shield->health);
        int freq = shield->data.shields.frequency;
        int temp_y = 50 * i + viewer_y + 20;
        int temp_x = cur_x + 100;
        gfx_SetColor(37);
        if(!shield->online) gfx_SetColor(128);
        gfx_FillRectangle(temp_x, temp_y, 160, 40);
        gfx_SetColor(1);
        gfx_FillRectangle(temp_x + 3, temp_y + 3, 154, 34);
        temp_x += 5; temp_y += 8;
        gfx_RLETSprite(icon_shieldstrength, temp_x, temp_y - 2);
        gfx_SetTextXY(temp_x + 15, temp_y);
        gfx_PrintUInt(resist, num_GetLength(resist));
        temp_x += 40;
        gfx_RLETSprite(icon_frequency, temp_x, temp_y - 2);
        gfx_SetTextXY(temp_x + 15, temp_y);
        gfx_PrintUInt(freq, num_GetLength(freq));
        temp_y += 15;
        temp_x = cur_x + 105;
        Stats_DrawHealthBar(health, 80, temp_x, temp_y, 255, 255, true);
        if(shield->online){
            shields_combined += health;
            shields_active++;
        }
    }
    if(shields_active) {
        gfx_rletsprite_t* shield_icon = icon_shieldsstable;
        shields_combined /= shields_active;
        if(shields_combined <= 50) shield_icon = icon_shieldscollapse;
        if(shields_combined <= 25) shield_icon = icon_shieldsfail;
        gfx_RLETSprite(shield_icon, cur_x, cur_y);
        cur_y += shield_icon->height + 10;
        Stats_DrawHealthBar(shields_combined, 75, cur_x, cur_y, 30, 255, true);
        
    }
}



void Screen_UIRepairSys(module_t* modules, unsigned char count){
    unsigned char i;
    gfx_PrintStringXY("[REPAIR SYSTEM]", viewer_x + 70, viewer_y);
    for(i = 0; i < count; i++){
        // loop module health display
        module_t* module = &modules[i];
        gfx_SetTextXY(viewer_x, (i + 1) * 10 + viewer_y + 5);
        if(!module->techclass){
            gfx_PrintString("module unloaded");
        }
        else {
            signed int health = power_GetBatteryPercent(&module->power);
            gfx_PrintUInt(health, 3);
        }
    }
    return;
}











int Stats_DrawHealthBar(unsigned int percent, unsigned int length, int x, int y, unsigned char border_color, unsigned char bar_color, bool icon){
    if(icon) {
        gfx_RLETSprite(icon_health, x, y - 2);
        x += 15;
    }
    gfx_SetColor(border_color);
    gfx_Rectangle(x, y-1, length, 9);
    length -= 2;
    gfx_SetColor(bar_color);
    if(percent <= 50) gfx_SetColor(229);
    if(percent <= 25) gfx_SetColor(192);
    gfx_FillRectangle(x + 1, y, percent * length / 100, 7);
    gfx_SetColor(255);
    return x + length;
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
    gfx_PrintStringXY("Shield", 12, lcars_texty);
    gfx_PrintStringXY("Tact", 80, lcars_texty);
    gfx_PrintStringXY("Mains", 140, lcars_texty);
    gfx_PrintStringXY("Repair", 200, lcars_texty);
    gfx_PrintStringXY("Cargo", 268, lcars_texty);
    gfx_SetTextFGColor(148);
    gfx_PrintStringXY(version, 160 - (7 * strlen(version) / 2), 0);
    gfx_SetTextFGColor(0);
}
