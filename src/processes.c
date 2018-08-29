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

void PROC_PowerCycle(Module_t *ShipModules, Module_t *warpcore, Module_t *auxpower, char repairing, Player_t *player, bool powerwarp){
    // process energy production and charge modules
    unsigned char i;
    char warphealth, drawreduction = 100;
    int powerout;
    bool status = true;
    if(warpcore->techtype == tt_warpcore && warpcore->online && powerwarp == true){
        warphealth = warpcore->health * 100 / warpcore->maxHealth;
        powerout = warpcore->stats.sysstats.powerOut;
        if(warphealth < 75) powerout = powerout * 3 / 4;
        if(warphealth < 50) {powerout = powerout / 2; drawreduction = 80;}
        if(warphealth < 25) {powerout = powerout / 4; drawreduction = 60;}
        if(warphealth == 0 || !warpcore->online) powerout = 0;
        warpcore->stats.sysstats.powerOutActual = powerout;
        if(drawreduction < 100) status = false;
    }
    else {
        powerout = auxpower->powerReserve;
        auxpower->powerReserve = 0;
        player->powersource = 1;
    }
    for(i = 0; i < 15; i++){
        Module_t *module = &ShipModules[i];
           // gfx_DrawPowerStatusIcon(false);
        if(!powerout) break;
        if(module->techtype && (module->techtype <= tt_transporter) && module->powerReserve < 256 && i != repairing){
            if(drawreduction == 100 && (module->powerDraw == module->powerDefault))
                drawreduction = (module->powerReserve < 256) ? 120 : 100;
            powerout -= (module->powerDefault * drawreduction / 100);
            module->powerReserve += (module->powerDefault * drawreduction / 100);
            if(module->powerReserve < 64) status = false;
            if(module->powerReserve >= 256) module->powerReserve = 256;
        }
    }
    auxpower->powerReserve += powerout;
    if(auxpower->powerReserve > auxpower->stats.sysstats.auxiliarymax)
        auxpower->powerReserve = auxpower->stats.sysstats.auxiliarymax;
    if(auxpower->powerReserve < 0)
        auxpower->powerReserve = 0;
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
        if(module->techtype){
            if(module->online == true && module->pdConstant){
               module->powerReserve -= module->powerDraw;
               if(module->powerReserve <= 0) Module_NoPower(module);
            }
            else if(i == repairing && module->health < module->maxHealth){
                module->powerReserve -= module->powerDraw;
                if(module->powerReserve <= 0) Module_NoPower(module);
            }
        }
    }
}

void proc_MoveShip(Position_t *pos){
    int coord_x = pos->coords.x, coord_y = pos->coords.y, coord_z = pos->coords.z;
    int vector_x = pos->vectors.x, vector_y = pos->vectors.y, vector_z = pos->vectors.z;
    coord_x -= vector_x;
    coord_y -= vector_y;
    coord_z -= vector_z;
    pos->coords.x = coord_x;
    pos->coords.y = coord_y;
    pos->coords.z = coord_z;
}
