#include <graphx.h>
#include <decompress.h>
#include "datatypes/shipmodules.h"
#include "gfx_functions.h"
#include "gfx/icons.h"
#include "equates.h"


void GUI_PowerReport(Module_t *ShipModules) {
    char counter = 1, i;
    gfx_WipeScreen();
    PrintHeader("Power Allocation", xStart+1, yStart+1, 80, 2);
    gfx_PrintStringXY("Reserve", xStart+90, yStart+18);
    gfx_PrintStringXY("Usage", xStart+195, yStart+18);
    
    for(i = 0; i < 20; i++){
        Module_t *module = &ShipModules[i];
        if(module->modtype != 0 && module->techtype != tt_warpcore){
            int currpower = module->powerReserve*100/255;
            int powersett = module->powerDraw*100/module->powerDefault;
            gfx_PrintStringXY(module->techname, xStart+5, yStart+((counter+2)*9));
           
            lcars_DrawHealthBar(currpower, 2, xStart+80, yStart+((counter+2)*9), true);
            lcars_DrawHealthBar(powersett, 4, xStart+177, yStart+((counter+2)*9), true);
            gfx_Line(xStart+176, yStart+19, xStart+176, yStart+((counter+3)*9));
            counter++;
        }
    }
    gfx_BlitBuffer();
}


void GUI_StatusReport(Module_t *ShipModules) {
    char counter = 1, i;
    gfx_WipeScreen();
    PrintHeader("Systems Status", xStart+1, yStart+1, 80, 2);
    
    
    for(i = 0; i < 20; i++){
        Module_t *module = &ShipModules[i];
        if(module->modtype != 0 && module->modtype != mt_shield){
            int health = module->health*100/module->maxHealth;
            gfx_PrintStringXY(module->techname, xStart+25, yStart+((counter+1)*9));
            lcars_DrawHealthBar(health, 1, xStart+120, yStart+((counter+1)*9), true);
            gfx_Line(xStart+228, yStart+17, xStart+228, yStart+((counter+2)*9));
            counter++;
        }
    }
    gfx_BlitBuffer();
}

void GUI_TacticalReport(Module_t *ShipModules, Module_t *shields){
    // shields GUI
    gfx_sprite_t *uncompressed;
   // Module_t *shield = &ShipModules[0];         // placeholder, prgm will look for shield
    int power = shields->powerDraw * 100 / shields->powerDefault;
    int resist = shields->stats.shieldstats.resistance;
    uint8_t modulation = shields->stats.shieldstats.modulation;
    gfx_WipeScreen();
    PrintHeader("Tactical Report", xStart+1, yStart+1, 80, 2);
    if(uncompressed = gfx_MallocSprite(tactical_width, tactical_height)){
        dzx7_Standard(tactical_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, xStart + 21, yStart + 25);
        free(uncompressed);
    }
    gfx_SetColor(37);
    if(!shields->online || (shields->health * 100 / shields->maxHealth) == 0) gfx_SetColor(224);
    gfx_Circle(xStart + 30, yStart + 45, 25);
    gfx_Circle(xStart + 30, yStart + 45, 26);
    gfx_Circle(xStart + 30, yStart + 45, 27);
    gfx_PrintStringXY("DRV: ", xStart + 65, yStart + 30);
    gfx_PrintUInt(resist, 1 + (resist>9) + (resist>99));
    gfx_PrintString(" kJ");
    gfx_PrintStringXY("POW: ", xStart + 65, yStart + 40);
    gfx_PrintUInt(power, 1 + (power>9) + (power>99));
    gfx_PrintString("%");
    gfx_PrintStringXY("MOD: ", xStart + 140, yStart + 35);
    gfx_PrintUInt(modulation, 1 + (modulation>9) + (modulation>99));
    gfx_PrintString(" mHz");
    lcars_DrawHealthBar(shields->health * 100 / shields->maxHealth, 1, xStart + 61, yStart + 53, true);
    gfx_BlitBuffer();
}
