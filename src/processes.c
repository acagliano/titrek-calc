#include <stdbool.h>
#include <stddef.h>
#include <compression.h>
#include <graphx.h>
#include "datatypes/shipmodules.h"
#include "gfx_functions.h"
#include "gfx/icons.h"
#include "mymath.h"

void Module_NoPower(Module_t* module){
    module->powerReserve = 0;
    module->online = false;
}

void PROC_PowerCycle(Module_t *ShipModules, Module_t *warpcore, char repairing, Player_t *player){
    // process energy production and charge modules
    unsigned char i;
    char powerout, warphealth;
    bool status = true;
    if(warpcore == NULL) return;
    warphealth = warpcore->health * 100 / warpcore->maxHealth;
    powerout = warpcore->stats.sysstats.powerOut;
    if(warphealth < 75) powerout = powerout * 3 / 4;
    if(warphealth < 50) powerout = powerout / 2;
    if(warphealth < 25) powerout = powerout / 4;
    if(warphealth == 0 || !warpcore->online) powerout = 0;
    for(i = 0; i <= 20; i++){
        Module_t *module = &ShipModules[i];
        gfx_SetColor(148);
        gfx_FillRectangle(310-power_alert_width, 203, power_alert_width, power_alert_height);
        if(powerout <= 0) {
           // gfx_DrawPowerStatusIcon(false);
            gfx_sprite_t *uncompressed;
            if(uncompressed = gfx_MallocSprite(power_alert_width, power_alert_height)){
                zx7_Decompress(uncompressed, power_alert_compressed);
                gfx_Sprite(uncompressed, 310 - power_alert_width, 203);
                free(uncompressed);
                gfx_BlitBuffer();
            }
            break;
        }
        if(module->techtype && module->techtype != tt_ejectedcore && module->powerReserve < 256 && i != repairing){
            powerout -= module->powerDefault;
            module->powerReserve += module->powerDefault;
            if(module->powerReserve < 64) status = false;
            if(module->powerReserve >= 256) module->powerReserve = 256;
        }
    }
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
            module->techtype == tt_sensor)) ||
           (i == repairing)
           ){
               module->powerReserve -= module->powerDraw;
               if(module->powerReserve <= 0) Module_NoPower(module);
        }
    }
}


void PROC_AnglesToVectors(Position_t *pos){
    char xzangle = pos->angles[0], yangle = pos->angles[1];
    pos->vectors[0] = byteCos(xzangle) * byteCos(yangle);
    pos->vectors[1] = byteSin(xzangle) * byteCos(yangle);
    pos->vectors[2] = byteSin(yangle);
    //pos->vectors[2] = z vector
}
