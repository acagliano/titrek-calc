#include <graphx.h>
#include <compression.h>
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "gfx_functions.h"
#include "gfx/icons.h"
#include "equates.h"


void GUI_PowerReport(Module_t *ShipModules, char selected, char speed) {
    char counter = 1, i;
    gfx_WipeScreen();
    PrintHeader("Power Allocation", xStart+1, yStart+1, 80, 2);
    gfx_PrintStringXY("Reserve", xStart+90, yStart+18);
    gfx_PrintStringXY("Usage", xStart+195, yStart+18);
    
    for(i = 0; i < 15; i++){
        Module_t *module = &ShipModules[i];
        if(module->techtype){
            int currpower = module->powerReserve*100/255;
            int powersett = module->powerDraw*100/module->powerDefault;
            gfx_PrintStringXY(module->techname, xStart+10, yStart+((counter+2)*9));
           
            lcars_DrawHealthBar(currpower, 2, xStart+80, yStart+((counter+2)*9), true);
            if(module->online) {
                if((module->techtype == tt_warpdrive && speed < 5) || (module->techtype == tt_impulsedrive && (speed == 0 || speed > 4))){
                    gfx_SetTextFGColor(229);
                    gfx_PrintStringXY("inactive", xStart+185, yStart+((counter+2)*9));
                } else if(module->techtype == tt_ejectedcore){
                    gfx_SetTextFGColor(229);
                    gfx_PrintStringXY("ejected", xStart+185, yStart+((counter+2)*9));
                } else lcars_DrawHealthBar(powersett, 4, xStart+177, yStart+((counter+2)*9), true);
            } else {
                gfx_SetTextFGColor(224);
                gfx_PrintStringXY("offline", xStart+185, yStart+((counter+2)*9));
            }
            gfx_SetTextFGColor(255);
            gfx_Line(xStart+176, yStart+19, xStart+176, yStart+((counter+3)*9));
            counter++;
        }
    }
    gfx_SetColor(247);
    gfx_FillRectangle(xStart+5, yStart+(selected+3)*9, 4, 7);
}


void GUI_StatusReport(Module_t *ShipModules, char selected, char repairing) {
    char counter = 1, i;
    gfx_WipeScreen();
    PrintHeader("Systems Status", xStart+1, yStart+1, 80, 2);
    
    
    for(i = 0; i < 15; i++){
        Module_t *module = &ShipModules[i];
        if(module->techtype){
            int health = module->health*100/module->maxHealth;
            gfx_SetColor(36);
            if(!module->online) gfx_SetColor(224);
            if(repairing == i) gfx_SetColor(229);
            gfx_Circle(xStart+18, yStart+3+((counter+1)*9), 1);
            gfx_Circle(xStart+18, yStart+3+((counter+1)*9), 2);
            gfx_PrintStringXY(module->techname, xStart+25, yStart+((counter+1)*9));
            if(module->techtype == tt_ejectedcore) gfx_PrintStringXY("core ejected", xStart+120, yStart+((counter+1)*9));
            else lcars_DrawHealthBar(health, 1, xStart+120, yStart+((counter+1)*9), true);
            gfx_Line(xStart+228, yStart+17, xStart+228, yStart+((counter+2)*9));
            counter++;
        }
    }
    gfx_SetColor(247);
    gfx_FillRectangle(xStart+10, yStart+(selected+2)*9, 4, 7);
    if(repairing != -1) gfx_FillRectangle(xStart+4, (repairing+2)*9+1+yStart, 7, 5);
}

void GUI_TacticalReport(Module_t *ShipModules, Module_t *shields, Module_t *activeweap){
    // shields GUI
    gfx_sprite_t *uncompressed;
    char i;
    unsigned char modulation = shields->stats.shieldstats.modulation;
   // Module_t *shield = &ShipModules[0];         // placeholder, prgm will look for shield
    int power = shields->powerDraw * 100 / shields->powerDefault;
    int resist = shields->stats.shieldstats.resistance;
    gfx_WipeScreen();
    PrintHeader("Tactical Report", xStart+1, yStart+1, 80, 2);
    gfx_SetColor(30);
    gfx_SetTextBGColor(30);
    gfx_FillRectangle(xStart+1, yStart+19, 11, 65);
    gfx_PrintStringXY("S", xStart+3, yStart+ 21);
    gfx_PrintStringXY("H", xStart+3, yStart+ 30);
    gfx_PrintStringXY("I", xStart+3, yStart+ 39);
    gfx_PrintStringXY("E", xStart+3, yStart+ 48);
    gfx_PrintStringXY("L", xStart+3, yStart+ 56);
    gfx_PrintStringXY("D", xStart+3, yStart+ 65);
    gfx_PrintStringXY("S", xStart+3, yStart+ 74);
     gfx_SetTextBGColor(0);
    if(uncompressed = gfx_MallocSprite(tactical_width, tactical_height)){
        zx7_Decompress(uncompressed, tactical_compressed);
        gfx_TransparentSprite(uncompressed, xStart + 41, yStart + 30);
        free(uncompressed);
    }
    gfx_SetColor(37);
    if(shields->health * 100 / shields->maxHealth < 25) gfx_SetColor(226);
    if(!shields->online || shields->health == 0) gfx_SetColor(224);
    gfx_Circle(xStart + 50, yStart + 50, 25);
    gfx_Circle(xStart + 50, yStart + 50, 26);
    gfx_Circle(xStart + 50, yStart + 50, 27);
    gfx_PrintStringXY("DRV: ", xStart + 85, yStart + 35);
    gfx_PrintUInt(resist, 1 + (resist>9) + (resist>99));
    gfx_PrintString(" kJ");
    gfx_PrintStringXY("POW: ", xStart + 85, yStart + 45);
    gfx_PrintUInt(power, 1 + (power>9) + (power>99));
    gfx_PrintString("%");
    gfx_PrintStringXY("STATE: ", xStart + 160, yStart + 35);
    if(shields->online) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
    gfx_PrintStringXY("MOD: ", xStart + 160, yStart + 45);
    gfx_PrintUInt(modulation, 1 + (modulation>9) + (modulation>99));
    gfx_PrintString(" mHz");
    lcars_DrawHealthBar(shields->health * 100 / shields->maxHealth, 1, xStart + 81, yStart + 58, true);
    
    gfx_SetColor(224);
    gfx_SetTextBGColor(224);
    gfx_FillRectangle(xStart+1, yStart+90, 11, 65);
    gfx_PrintStringXY("W", xStart+3, yStart+ 92);
    gfx_PrintStringXY("E", xStart+3, yStart+ 101);
    gfx_PrintStringXY("A", xStart+3, yStart+ 110);
    gfx_PrintStringXY("P", xStart+3, yStart+ 119);
    gfx_PrintStringXY("O", xStart+3, yStart+ 128);
    gfx_PrintStringXY("N", xStart+3, yStart+ 137);
    gfx_PrintStringXY("S", xStart+3, yStart+ 146);
    gfx_SetTextBGColor(0);
    for(i = tt_phaser - 1; i < tt_torpedo; i++){
        Module_t *weap = &ShipModules[i];
        bool armed = false;
        char damage_shield = weap->stats.weapstats.damage_shield,
        damage_hull = weap->stats.weapstats.damage_hull;
        int rect_start_x = (i-5)*(vWidth>>1)+xStart+20, rect_start_y = yStart+92;
        if(i == tt_torpedo - 1) rect_start_x -= 10;
        gfx_SetColor(224);
        if(weap == activeweap){
            gfx_SetColor(36);
        }
        gfx_FillRectangle(rect_start_x, rect_start_y, vWidth/2 - 20, 60);
        gfx_SetColor(181);
        gfx_SetTextFGColor(0);
        gfx_SetTextBGColor(181);
        gfx_FillRectangle(rect_start_x+3, rect_start_y+3, vWidth/2 - 26, 54);
        gfx_PrintStringXY("Class: ", rect_start_x + 5, rect_start_y + 5);
        if(weap->techtype == tt_phaser) gfx_PrintString("Phaser");
        else if(weap->techtype == tt_torpedo) gfx_PrintString("Torpedo");
        gfx_PrintStringXY("Type: ", rect_start_x + 5, rect_start_y + 15);
        gfx_PrintString(weap->stats.weapstats.weapname);
        gfx_PrintStringXY("_Damage_", rect_start_x + 5, rect_start_y + 25);
        gfx_PrintStringXY("Shields: ", rect_start_x + 12, rect_start_y + 36);
        gfx_PrintStringXY("Hull:    ", rect_start_x + 12, rect_start_y + 46);
        gfx_SetTextXY(rect_start_x + 70, rect_start_y + 36);
        gfx_PrintUInt(damage_shield, 1 + (damage_shield>9)/* + (modulation>99)*/);
        gfx_SetTextXY(rect_start_x + 70, rect_start_y + 46);
        gfx_PrintUInt(damage_hull, 1 + (damage_hull>9)/* + (modulation>99)*/);
        
    }
}
