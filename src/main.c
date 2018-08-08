//--------------------------------------
// Program Name: Star Trek Multplayer
// Author: Anthony Cagliano
// License:
// Description:
//--------------------------------------

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h, debug.h
#include <fileioc.h>
#include <graphx.h>
#include <decompress.h>



/* Put your function prototypes here */
void printText(const char *text, uint8_t x, uint8_t y);

#include "gfx/icons.h"
#include "gfx_functions.h"
#include "datatypes/mapdata.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "processes.h"
#include "initialize.h"
#include "mapfuncs.h"

#include "screens.h"

#include "equates.h"

Player_t player[1] = {0};
Module_t ShipModules[20] = {0};
MapData_t* MapMain;
Module_t *shields = NULL, *integrity = NULL, *auxpower = NULL, *lifesupport = NULL, *warpcore = NULL, *warpdrive = NULL;
const char *GameSave = "TrekSave";

void main(void) {
    char SavePtr;
    char i;
    char key = 0;
    char mapslot;
    MapMain = calloc(10, sizeof(MapData_t));
    ti_CloseAll();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(1);
    gfx_SetTransparentColor(0);
	/* Fill in the body of the main function here */
    for(i = 0; i < 8; i++){
        Module_t *module = &ShipModules[i];
        module->online = true;
        module->techtype = i;
        module->health = 50;
        module->maxHealth = 50;
        module->powerReserve = 256;
        module->powerDraw = 5;
        module->powerDefault = 5;
        switch(i){
            case tt_shield:
                module->modtype = mt_shield;
                strcpy(module->techname, "Shields");
                module->stats.shieldstats.resistance = 5;
                module->stats.shieldstats.modulation = randInt(0, 255);
                break;
            case tt_integrity:
                module->modtype = mt_system;
                module->stats.sysstats.resistance = 1;
                strcpy(module->techname, "Integrity");
                break;
            case tt_lifesupport:
                module->modtype = mt_system;
                strcpy(module->techname, "LifeSupp");
                break;
            case tt_warpcore:
                module->modtype = mt_system;
                strcpy(module->techname, "WarpCore");
                module->powerReserve = 256;
                module->stats.sysstats.powerOut = 50;
                module->online = true;
                break;
            case tt_warpdrive:
                module->modtype = mt_system;
                module->stats.sysstats.topSpeed = 6;
                strcpy(module->techname, "WarpDrive");
                break;
            case tt_impulsedrive:
                module->modtype = mt_system;
                strcpy(module->techname, "ImpulseDr");
                break;
            case tt_transporter:
                module->modtype = mt_system;
                strcpy(module->techname, "Transport");
                break;
            case tt_sensor:
                module->modtype = mt_system;
                strcpy(module->techname, "Sensors");
                break;
            default:
                module->modtype = mt_system;
                strcpy(module->techname, "Unsupport");
        }
    }
    shields = init_SetPointer(&ShipModules, 0);   // return pointer to shields
    integrity = init_SetPointer(&ShipModules, 1);   // return pointer to hull integrity
    auxpower = init_SetPointer(&ShipModules, 2);   // return pointer to auxiliary
    warpcore = init_SetPointer(&ShipModules, 3);   // return pointer to core
    lifesupport = init_SetPointer(&ShipModules, 4);   // return pointer to life support
    warpdrive = init_SetPointer(&ShipModules, 5);   // return pointer to warpdrive
    boot_ClearVRAM();
    DrawGUI(&ShipModules);
    player->ScreenSelected = SCRN_POWER;
    player->timers[timer_power] = POWER_INTERVAL;
    player->moduleRepairing = -1;
    if((mapslot = map_LocateSlot(MapMain)) >= 0){
        MapData_t *slot = &MapMain[mapslot];
        // assign ship data to slot
        slot->entitytype = et_ship;
        slot->mobile = false;
        slot->entitystats.e_ship.health = 50;
        slot->entitystats.e_ship.maxHealth = 50;
        slot->entitystats.e_ship.shield = 50;
        slot->entitystats.e_ship.shieldMax = 50;
        slot->position.coords[0] = randInt(0,256) << 8;
        slot->position.coords[1] = randInt(0,256) << 8;
        slot->position.coords[2] = randInt(0,256) << 8;
    }
    do
    {
        char corestability, lifesupporthealth, speed = player->position.speed;
        char topspeed = warpdrive->stats.sysstats.topSpeed;
        unsigned char warppow = warpdrive->powerDraw * 100 / warpdrive->powerDefault;
        if( warppow > 100) topspeed += (warppow / 10 - 10) / 2;
        else topspeed = topspeed * warppow / 100;
        if(topspeed < 5) topspeed = 5;
        if(speed > topspeed) {
            player->position.speed = topspeed;
            speed = topspeed;
        }
        key = os_GetCSC();
        
       
        if(key == sk_Clear) break;
       
        if( key == sk_Down){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[1]++;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                case SCRN_STATUS:
                    player->moduleSelected++;
                    module = &ShipModules[player->moduleSelected];
                    if(!module->modtype) player->moduleSelected--;
                    break;
            }
        }
        
        if( key == sk_Up){
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[1]--;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                case SCRN_STATUS:
                    player->moduleSelected--;
                    if(player->moduleSelected < 0) player->moduleSelected++;
                    break;
            }
        }
        
        if( key == sk_Right){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[0]++;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                    module = &ShipModules[player->moduleSelected];
                    if(module->techtype != tt_warpcore){
                        module->powerDraw++;
                        if(module->powerDraw > 2 * module->powerDefault) module->powerDraw--;
                    }
                    break;
            }
        }
        
        if( key == sk_Left){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[0]--;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                    module = &ShipModules[player->moduleSelected];
                    if(module->techtype != tt_warpcore){
                        module->powerDraw--;
                        if(module->powerDraw < 1) module->powerDraw++;
                    }
                    break;
            }
        }
        
        if(key == sk_Mul){
            if(player->position.speed < 5) player->position.speed = 5;
            else player->position.speed = 0;
        }
        
        
        if( key == sk_Add && speed < 54)
            if(speed < topspeed)
                if(speed < 4 || speed >= 5) player->position.speed++;
        
        if( key == sk_Sub && speed > 0)
            if(speed > 5 || speed <= 4) player->position.speed--;
        
        if( key == sk_Yequ ){
            if(player->ScreenSelected == SCRN_TACTICAL) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_TACTICAL;
        }
        
        if( key == sk_Mode ){
            if(player->ScreenSelected == SCRN_POWER){
                Module_t* module = &ShipModules[player->moduleSelected];
                module->online = !module->online;
            }
            if(player->ScreenSelected == SCRN_STATUS){
                Module_t* module = &ShipModules[player->moduleSelected];
                if(player->moduleRepairing == player->moduleSelected){
                    player->moduleRepairing = -1;
                    module->online = true;
                }
                else {
                    player->moduleRepairing = player->moduleSelected;
                    module->online = false;
                    module->powerDraw = module->powerDefault;
                }
            }
        }
        
        if( key == sk_Del ){
            Module_t *module = &ShipModules[player->moduleSelected];
            if(module->techtype == tt_warpcore && player->timers[timer_corebreach]){
                module->modtype = mt_ejectedcore;
                player->timers[timer_corebreach] = 0;
                warpcore = NULL;
            }
        }
        
        if( key == sk_Window ){
            if(player->ScreenSelected == SCRN_STATUS) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_STATUS;
        }
        
        if( key == sk_Trace ){
            if(player->ScreenSelected == SCRN_POWER) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_POWER;
        }
        
        if( key == sk_Power){
            //damage random system
            char i;
            char damage = randInt(4,6);
            char drv = shields->stats.shieldstats.resistance * shields->powerDraw * shields->health / shields->powerDefault / shields->maxHealth;
            char int_drv = integrity->health * integrity->powerDraw * 100 / integrity->maxHealth / integrity->powerDefault;
            if(shields->online){
                shields->health -= damage;
                if(shields->health < 0) shields->health = 0;
                damage -= drv;
                if(damage < 0) damage = 0;
            }
            drv = 0;
            if(integrity->online){
                if(int_drv >= 50) drv = integrity->stats.sysstats.resistance;
                if(int_drv < 50) drv = 0;
                if(!int_drv) drv = 0 - integrity->stats.sysstats.resistance;
                integrity->health -= damage;
                if(integrity->health < 0) integrity->health = 0;
                damage -= drv;
                if(damage < 0) damage = 0;
            }
            for(i = 0; i < 2; i++){
                Module_t* module = &ShipModules[randInt(2,12)];
                module->health -= damage;
                if(module->health <= 0) module->health = 0;
            }
        }
        
        
        switch(player->ScreenSelected){
            case SCRN_VIEW:
                GUI_ViewScreen(MapMain, &player->position);
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, shields);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules, player->moduleSelected, player->moduleRepairing);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules, player->moduleSelected, player->position.speed);
                break;
        }
        if(player->timers[timer_power] == 0){
            PROC_PowerDraw(&ShipModules, player->moduleRepairing);
            PROC_PowerCycle(&ShipModules, warpcore, player->moduleRepairing, &player);
            player->timers[timer_power] = POWER_INTERVAL;
        }
        else player->timers[timer_power]--;
        
        if(player->timers[timer_repair] == 0){
            if(player->moduleRepairing != -1){
                Module_t* repair = &ShipModules[player->moduleRepairing];
                if(!repair->online && repair->health < repair->maxHealth && repair->powerReserve)
                    repair->health++;
                player->timers[timer_repair] = REPAIR_INTERVAL;
            }
        }
        else player->timers[timer_repair]--;
        
       
        
        corestability = warpcore->health * 100 / warpcore->maxHealth;
        lifesupporthealth = lifesupport->health * 100 / lifesupport->maxHealth;
        if(warpcore){
            if(corestability <= 25){
                if(randInt(0, corestability * 10) == 0 && !player->timers[timer_corebreach])
                    player->timers[timer_corebreach]++;
            } else if(corestability > 50) player->timers[timer_corebreach] = 0;
            if(player->timers[timer_corebreach]) player->timers[timer_corebreach]++;
            if(player->timers[timer_corebreach] == CORE_BREACH_TIMER){
                player->deathreason = 1;
                break;
            }
        }
        if((lifesupporthealth == 0 || !lifesupport->online) & !player->timers[timer_lifesupport])
            player->timers[timer_lifesupport]++;
        else if(lifesupporthealth > 50 && lifesupport->online) player->timers[timer_lifesupport] = 0;
        if(player->timers[timer_lifesupport]) player->timers[timer_lifesupport]++;
        if(player->timers[timer_lifesupport] == LIFE_SUPPORT_TIMER){
            player->deathreason = 2;
            break;
        }
        gfx_DrawLifeSupportAlert(player->timers[timer_lifesupport] != 0);
        gfx_DrawCoreBreachAlert(player->timers[timer_corebreach] != 0);
        gfx_DrawShipStatusIcon(integrity, shields);
        gfx_DrawSpeedIndicator(player->position.speed, topspeed);
        gfx_BlitBuffer();
    }
    while(key != sk_Clear);
    gfx_SetDrawScreen();
    boot_ClearVRAM();
    
    
    // Cleanup
    ti_CloseAll();
    gfx_SetTextFGColor(0);
    gfx_SetTextBGColor(255);
    switch(player->deathreason){
        case 1:
            gfx_PrintStringXY("Warp Core has breached!", 1, 0);
            break;
        case 2:
            gfx_PrintStringXY("Life Support has failed!", 1, 0);
            break;
    }
    gfx_PrintStringXY("Thanks for testing!!", 1, 10);
    os_GetKey();
    gfx_End();
	prgm_CleanUp();
}

/* Put other functions here */
/* Draw text on the homescreen at the given X/Y location */
void printText(char *text, uint8_t xpos, uint8_t ypos) {
    os_SetCursorPos(ypos, xpos);
    os_PutStrFull(text);
}


