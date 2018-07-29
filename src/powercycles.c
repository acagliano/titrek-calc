#include <stdbool.h>
#include "datatypes/shipmodules.h"
#include "gfx_functions.h"

void Module_NoPower(Module_t* module){
    module->powerReserve = 0;
    module->online = false;
}

void PROC_PowerCycle(Module_t *ShipModules, char repairing){
    // process energy production and charge modules
    unsigned char i;
    char powerout;
    bool status = true;
    Module_t* module;
    for(i = 0; i <= 20; i++){
        module = &ShipModules[i];
        if(module->online == true && module->techtype == tt_warpcore) break;
    }
    if(i > 20) return;
    powerout = module->stats.sysstats.powerOut;
    if(module->health * 100 / module->maxHealth < 75) powerout = powerout * 3 / 4;
    if(module->health * 100 / module->maxHealth < 50) powerout = powerout / 2;
    if(module->health * 100 / module->maxHealth < 25) powerout = powerout / 4;
    if(module->health == 0 || !module->online) powerout = 0;
    for(i = 0; i <= 20; i++){
        module = &ShipModules[i];
        if(module->modtype && module->powerReserve < 256 &&
           (!repairing || (i+1 != repairing))
           ){
            powerout -= module->powerDefault;
            module->powerReserve += module->powerDefault;
            if(module->powerReserve < 64) status = false;
            if(module->powerReserve >= 256) module->powerReserve = 256;
            if(powerout < 0) {
                status = false;
                break;
            }
        }
    }
    gfx_DrawPowerStatusIcon(status);
}

void PROC_PowerDraw(Module_t *ShipModules, char repairing){
    // process module energy expenditure
    unsigned char i;
    for(i = 0; i <= 20; i++){
        Module_t* module = &ShipModules[i];
        if((module->online == true &&
           (module->techtype == tt_shield ||
            module->techtype == tt_integrity ||
            module->techtype == tt_lifesupport ||
            module->techtype == tt_sensor ||
            module->techtype == tt_comms)) ^
           (repairing && (i+1 == repairing))
           ){
               module->powerReserve -= module->powerDraw;
               if(module->powerReserve <= 0) Module_NoPower(module);
        }
    }
}
