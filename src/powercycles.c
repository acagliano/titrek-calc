#include <stdbool.h>
#include "datatypes/shipmodules.h"
#include "gfx_functions.h"

void Module_NoPower(Module_t* module){
    module->powerReserve = 0;
    module->online = false;
    module->powerDraw = module->powerDefault / 2;
}

void PROC_PowerCycle(Module_t *ShipModules){
    // process energy production and charge modules
    unsigned char i, onetime = 0;
    char powerout;
    bool status = true;
    Module_t* module;
    for(i = 0; i <= 20; i++){
        module = &ShipModules[i];
        if(module->online == true && module->techtype == tt_warpcore) break;
    }
    if(i > 20) return;
    powerout = module->stats.sysstats.powerOut * module->health / module->maxHealth;
    for(i = 0; i <= 20; i++){
        module = &ShipModules[i];
        if(module->modtype && module->powerReserve < 256 &&
           (module->techtype != tt_warpcore)
           ){
            powerout -= module->powerDefault;
            module->powerReserve += module->powerDefault;
            if(module->powerReserve < 64) status = false;
            if(module->powerReserve >= 128 && module->online == false){
                module->online = true;
            }
            if(module->powerReserve >= 256){
                module->powerReserve = 256;
                module->powerDraw = module->powerDefault;
            }
            if(powerout <= 0) break;
        }
    }
    gfx_DrawPowerStatusIcon(status);
}

void PROC_PowerDraw(Module_t *ShipModules){
    // process module energy expenditure
    unsigned char i;
    for(i = 0; i <= 20; i++){
        Module_t* module = &ShipModules[i];
        if(module->online == true &&
           (module->techtype == tt_shield ||
            module->techtype == tt_integrity ||
            module->techtype == tt_lifesupport ||
            module->techtype == tt_sensor ||
            module->techtype == tt_comms)){
            module->powerReserve -= module->powerDraw;
               if(module->powerReserve <= 0) Module_NoPower(module);
        }
    }
}
