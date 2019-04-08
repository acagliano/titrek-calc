#include <string.h>
#include "classes/ships.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include <graphx.h>
#include <compression.h>

char mainsys_strings[][10] = {"Integrity", "LifeSupp", "WarpCore", "WarpDrive", "Impulse", "NavSens", "Transport", "Comms"};

void Screen_UISysStats(module_t* system, unsigned char count){
    unsigned char i;
    gfx_PrintStringXY("[SYSTEM STATS]", viewer_x + 80, viewer_y);
    for(i = 0; i < count; i++){
        // loop module health display
        module_t* module = &system[i];
        gfx_SetTextXY(viewer_x, (i + 1) * 12 + viewer_y + 5);
        if(!module->techclass){
            gfx_PrintString("module unloaded");
        }
        else {
            signed int health = health_GetHealthPercent(&module->health);
            gfx_PrintString(mainsys_strings[module->data.techtype]);
            Stats_DrawHealthBar(health, 50, viewer_x + 100, (i + 1) * 12 + viewer_y + 5);
            //gfx_SetTextXY(viewer_x + 100, (i + 1) * 10 + viewer_y + 5);
            //gfx_PrintUInt(health, 3);
        }
    }
    return;
}

void Screen_UITactStats(module_t* tactical, unsigned char count){
    unsigned char i;
    gfx_PrintStringXY("[TACTICAL STATS]", viewer_x + 75, viewer_y);
    for(i = 0; i < count; i++){
        // loop module health display
        module_t* module = &tactical[i];
        gfx_SetTextXY(viewer_x, (i + 1) * 10 + viewer_y + 5);
        if(!module->techclass){
            gfx_PrintString("module unloaded");
        }
        else {
            signed int health = health_GetHealthPercent(&module->health);
            gfx_PrintUInt(health, 3);
        }
    }
    return;
}

void Screen_UIPowerStats(module_t* modules, unsigned char count){
    unsigned char i;
    gfx_PrintStringXY("[POWER STATS]", viewer_x + 75, viewer_y);
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











void Stats_DrawHealthBar(unsigned char percent, unsigned int length, int x, int y){
    gfx_SetColor(255);
    gfx_Rectangle(x, y-1, 50, 9);
    gfx_FillRectangle(x, y-1, percent * 50 / 100, 9);
    gfx_SetColor(0);
}



void Screen_Background(gfx_sprite_t** sprites, unsigned char active) {
    static char *version = "v0.81 pre-alpha";
    int i;
    gfx_SetTextFGColor(255);
    gfx_Sprite(sprites[0], 0, 0);
    gfx_Sprite(sprites[1], 160, 0);
    for(i = 0; i < 320; i++){
        gfx_SetColor(0);
        gfx_Rectangle(i * 64, 216, 64, 20);
    }
    gfx_SetColor(116);
    gfx_FillRectangle(active*64+1, 217, 62, 18);
    gfx_PrintStringXY("ViewScn", 5, lcars_texty);
    gfx_PrintStringXY("Tact Ctl", 69, lcars_texty);
    gfx_PrintStringXY("Sys Ctl", 137, lcars_texty);
    gfx_PrintStringXY("Repair", 200, lcars_texty);
    gfx_PrintStringXY("Cargo", 268, lcars_texty);
    gfx_SetTextFGColor(148);
    gfx_PrintStringXY(version, 160 - (7 * strlen(version) / 2), 0);
    gfx_SetTextFGColor(0);
}
