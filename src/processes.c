#include <stdbool.h>
#include <stddef.h>
#include <compression.h>
#include <graphx.h>
#include "datatypes/shipmodules.h"
#include "gfx_functions.h"
#include "gfx/icons.h"
#include "mymath.h"
#include "gfx/trekicon.h"

void Module_NoPower(Module_t* module){
    module->powerReserve = 0;
    module->online = false;
}

void PROC_PowerCycle(Module_t *ShipModules, Module_t *warpcore, char repairing, Player_t *player){
    // process energy production and charge modules
    unsigned char i;
    char powerout, warphealth, drawreduction = 100;
    bool status = true;
    if(warpcore == NULL) return;
    warphealth = warpcore->health * 100 / warpcore->maxHealth;
    powerout = warpcore->stats.sysstats.powerOut;
    if(warphealth < 75) powerout = powerout * 3 / 4;
    if(warphealth < 50) {powerout = powerout / 2; drawreduction = 80;}
    if(warphealth < 25) {powerout = powerout / 4; drawreduction = 60;}
    if(warphealth == 0 || !warpcore->online) powerout = 0;
    if(drawreduction < 100) status = false;
    for(i = 0; i < 15; i++){
        Module_t *module = &ShipModules[i];
           // gfx_DrawPowerStatusIcon(false);
        if(!powerout) break;
        if(module->techtype && (module->techtype <= tt_transporter) && module->powerReserve < 256 && i != repairing){
            if(drawreduction == 100 && (module->powerDraw == module->powerDefault)) drawreduction = (module->powerReserve < 256) ? 120 : 100;
            powerout -= (module->powerDefault * drawreduction / 100);
            module->powerReserve += (module->powerDefault * drawreduction / 100);
            if(module->powerReserve < 64) status = false;
            if(module->powerReserve >= 256) module->powerReserve = 256;
        }
    }
    gfx_SetColor(148);
    gfx_FillRectangle(310-power_alert_width, 203, power_alert_width, power_alert_height);
    if(!status){
        gfx_sprite_t *uncompressed;
        if(uncompressed = gfx_MallocSprite(power_alert_width, power_alert_height)){
            zx7_Decompress(uncompressed, power_alert_compressed);
            gfx_Sprite(uncompressed, 310 - power_alert_width, 203);
            free(uncompressed);
        }
    }
}

void PROC_PowerDraw(Module_t *ShipModules, char repairing){
    // process module energy expenditure
    unsigned char i;
    for(i = 0; i < 15; i++){
        Module_t* module = &ShipModules[i];
        if((module->online == true &&
           (module->pdConstant)) ||
           (i == repairing)
           ){
               module->powerReserve -= module->powerDraw;
               if(module->powerReserve <= 0) Module_NoPower(module);
        }
    }
}
