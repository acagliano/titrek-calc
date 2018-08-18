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
#include <compression.h>
#include <keypadc.h>



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
#include "mymath.h"
#include "vfx.h"

#include "screens.h"

#include "equates.h"

enum {
    k_Yequ,
    k_Window,
    k_Zoom,
    k_Trace,
    k_Graph,
    k_Mode,
    k_Del,
    k_Mul,
    k_Down,
    k_Up,
    k_Left,
    k_Right,
    k_Add,
    k_Sub,
    k_2nd
};
Player_t player[1] = {0};
Module_t ShipModules[20] = {0};
MapData_t* MapMain;
Module_t *shields = NULL, *integrity = NULL, *auxpower = NULL, *lifesupport = NULL, *warpcore = NULL, *warpdrive = NULL, *activeweap = NULL;
const char *GameSave = "TrekSave";

void main(void) {
    bool loopgame = true, key = false;
    bool keys_prior[15] = {0};
    char SavePtr;
    char i;
    char mapslot;
    MapMain = calloc(10, sizeof(MapData_t));
    srandom(rtc_Time());
    ti_CloseAll();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(1);
    gfx_SetTransparentColor(0);
	/* Fill in the body of the main function here */
    for(i = 1; i <= 10; i++){
        Module_t *module = &ShipModules[i-1];
        module->online = true;
        module->techtype = i;
        module->health = 50;
        module->maxHealth = 50;
        module->powerReserve = 256;
        module->powerDraw = 5;
        module->powerDefault = 5;
        switch(i){
            case tt_shield:
                strcpy(module->techname, "Shields");
                module->location = 0;
                module->stats.shieldstats.resistance = 5;
                module->stats.shieldstats.modulation = randInt(0, 255);
                break;
            case tt_integrity:
                module->location = 0;
                module->stats.sysstats.resistance = 1;
                strcpy(module->techname, "Integrity");
                break;
            case tt_lifesupport:
                module->location = aft;
                strcpy(module->techname, "LifeSupp");
                break;
            case tt_warpcore:
                module->location = aft;
                strcpy(module->techname, "WarpCore");
                module->powerReserve = 256;
                module->stats.sysstats.powerOut = 50;
                module->online = true;
                break;
            case tt_warpdrive:
                module->location = nacelles;
                module->stats.sysstats.topSpeed = 11;
                strcpy(module->techname, "WarpDrive");
                break;
            case tt_impulsedrive:
                module->location = aft;
                strcpy(module->techname, "ImpulseDr");
                break;
            case tt_transporter:
                module->location = saucer;
                strcpy(module->techname, "Transport");
                break;
            case tt_sensor:
                module->location = saucer;
                strcpy(module->techname, "Sensors");
                break;
            case tt_phaser:
                module->location = saucer;
                module->stats.weapstats.charge = 0;
                module->stats.weapstats.maxCharge = 10;
                module->stats.weapstats.damage_shield = 3;
                module->stats.weapstats.damage_hull = 1;
                module->stats.weapstats.range = 100;
                module->stats.weapstats.speed = 10;
                strcpy(module->techname, "Phasers");
                strcpy(module->stats.weapstats.weapname, "PulsPhas");
                break;
            case tt_torpedo:
                module->location = saucer;
                module->stats.weapstats.damage_shield = 1;
                module->stats.weapstats.damage_hull = 3;
                module->stats.weapstats.range = 100;
                module->stats.weapstats.speed = 5;
                strcpy(module->techname, "Torpedo");
                strcpy(module->stats.weapstats.weapname, "PhotTorp");
                break;
            default:
                strcpy(module->techname, "Unsupport");
        }
    }
    shields = init_SetPointer(&ShipModules, tt_shield, 0);   // return pointer to shields
    integrity = init_SetPointer(&ShipModules, tt_integrity, 0);   // return pointer to hull integrity
    auxpower = init_SetPointer(&ShipModules, tt_auxiliary, 0);   // return pointer to auxiliary
    warpcore = init_SetPointer(&ShipModules, tt_warpcore, 0);   // return pointer to core
    lifesupport = init_SetPointer(&ShipModules, tt_lifesupport, 0);   // return pointer to life support
    warpdrive = init_SetPointer(&ShipModules, tt_warpdrive, 0);   // return pointer to warpdrive
    activeweap = init_SetPointer(&ShipModules, tt_phaser, 0); // pointer to phasers
   
    boot_ClearVRAM();
    DrawGUI(&ShipModules);
    player->ScreenSelected = SCRN_STATUS;
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
        unsigned char warpdrivehealth = warpdrive->health * 100 / warpdrive->maxHealth;
        char topspeed = warpdrive->stats.sysstats.topSpeed;
        unsigned char warppow = warpdrive->powerDraw * 100 / warpdrive->powerDefault;
        if( warppow > 100) topspeed += (warppow / 10 - 10) / 2;
        else topspeed = topspeed * warppow / 100;
        topspeed = topspeed * warpdrivehealth / 100;
        if(topspeed < 10) topspeed = 10;
        if(warpdrivehealth == 0 || warppow == 0 || !warpdrive->online) topspeed = 4;
        if(speed > topspeed) {
            player->position.speed = topspeed;
            speed = topspeed;
        }
        if(activeweap != init_SetPointer(&ShipModules, tt_phaser, 0) || warpdrive != init_SetPointer(&ShipModules, tt_warpdrive, 0)) {
            player->deathreason = 3;
            break;
        }
        kb_ScanGroup(1);
        key = kb_Data[1] & kb_Window;
        if(key && !keys_prior[k_Window] ){
            if(player->ScreenSelected == SCRN_STATUS) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_STATUS;
        }
        keys_prior[k_Window] = key;
        key = kb_Data[1] & kb_Trace;
        if( key && !keys_prior[k_Trace] ){
            if(player->ScreenSelected == SCRN_POWER) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_POWER;
        }
        keys_prior[k_Trace] = key;
        key = kb_Data[1] & kb_Yequ;
        if( key && !keys_prior[k_Yequ] ){
            if(player->ScreenSelected == SCRN_TACTICAL) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_TACTICAL;
        }
        keys_prior[k_Yequ] = key;
        key = kb_Data[1] & kb_Mode;
        if( key && !keys_prior[k_Mode] ){
            if(player->ScreenSelected == SCRN_POWER){
                Module_t* module = &ShipModules[player->moduleSelected];
                module->online = !module->online;
            }
            if(player->ScreenSelected == SCRN_TACTICAL){
                char i = (activeweap == &ShipModules[6]) ? 7 : 6;
                activeweap = &ShipModules[i];
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
        keys_prior[k_Mode] = key;
        key = kb_Data[1] & kb_Del;
        if( key && !keys_prior[k_Del] ){
            Module_t *module = &ShipModules[player->moduleSelected];
            if(module->techtype == tt_warpcore && player->timers[timer_corebreach]){
                module->techtype = tt_ejectedcore;
                player->timers[timer_corebreach] = 0;
                warpcore = NULL;
            }
        }
        keys_prior[k_Del] = key;
        
        key = kb_Data[1] & kb_2nd;
        if( key && (!keys_prior[k_2nd] || player->tick % 5 == 0) ){
            if(activeweap && (activeweap->health > 0)){
                int power = activeweap->powerReserve;
                char maxCharge = activeweap->stats.weapstats.maxCharge;
                if((power >= (maxCharge * activeweap->powerDraw)) && (activeweap->stats.weapstats.charge < maxCharge)){
                    activeweap->stats.weapstats.charge++;
                }
            }
        }
        if(!key && keys_prior[k_2nd]){
            if(activeweap){ //fire phaser
                activeweap->powerReserve -= (activeweap->stats.weapstats.charge * activeweap->powerDraw);
                activeweap->stats.weapstats.charge = 0;
            }
        }
        keys_prior[k_2nd] = key;
        
        kb_ScanGroup(6);
        key = kb_Data[6] & kb_Mul;
        if( key && !keys_prior[k_Mul]){
            if(player->position.speed < 10) player->position.speed = 10;
            else player->position.speed = 0;
        }
        keys_prior[k_Mul] = key;
        key = kb_Data[6] & kb_Add;
        if( (key && (!keys_prior[k_Add] || player->tick % 5 == 0)) && speed < topspeed)
            if(speed < topspeed)
                if(speed < 4 || speed >= 10) player->position.speed++;
        keys_prior[k_Add] = key;
        key = kb_Data[6] & kb_Sub;
        if( (key && (!keys_prior[k_Sub] || player->tick % 5 == 0)) && speed > 0)
            if(speed > 10 || speed <= 4) player->position.speed--;
        keys_prior[k_Sub] = key;
        if( kb_Data[6] & kb_Power){
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
                Module_t* module = &ShipModules[randInt(2,7)];
                module->health -= damage;
                if(module->health <= 0) module->health = 0;
                if((module->health * 100 / module->maxHealth) < 50) {
                    if(module->location){
                        switch(module->location){
                            case nacelles:
                                player->damagesection[nacelles-1] = true;
                                break;
                            case aft:
                                player->damagesection[aft-1] = true;
                                break;
                            case saucer:
                                player->damagesection[saucer-1] = true;
                                break;
                        }
                    }
                }
            }
        }
        if(kb_Data[6] & kb_Clear) loopgame = false;
       
        kb_ScanGroup(7);
        key = kb_Data[7] & kb_Down;
        if( key ){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[1]++;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                case SCRN_STATUS:
                    if(key && !keys_prior[k_Down]){
                        player->moduleSelected++;
                        module = &ShipModules[player->moduleSelected];
                        if(!module->techtype) player->moduleSelected--;
                    }
                    break;
            }
        }
        keys_prior[k_Down] = key;
        key = kb_Data[7] & kb_Up;
        if( key ){
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[1]--;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                case SCRN_STATUS:
                    if(key && !keys_prior[k_Up]){
                        player->moduleSelected--;
                        if(player->moduleSelected < 0) player->moduleSelected++;
                    }
                    break;
            }
        }
        keys_prior[k_Up] = key;
        key = kb_Data[7] & kb_Right;
        if( key ){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles[0]++;
                    PROC_AnglesToVectors(&player->position);
                    break;
                case SCRN_POWER:
                    if(key && !keys_prior[k_Right]){
                        module = &ShipModules[player->moduleSelected];
                        if(module->techtype != tt_warpcore && module->techtype != tt_lifesupport){
                            module->powerDraw++;
                            if(module->powerDraw > 2 * module->powerDefault) module->powerDraw--;
                        }
                    }
                    break;
            }
        }
        keys_prior[k_Right] = key;
        key = kb_Data[7] & kb_Left;
        if( key){
        Module_t* module;
        switch(player->ScreenSelected){
            case SCRN_VIEW:
                player->position.angles[0]--;
                PROC_AnglesToVectors(&player->position);
                break;
            case SCRN_POWER:
                    if(key && !keys_prior[k_Left]){
                        module = &ShipModules[player->moduleSelected];
                        if(module->techtype != tt_warpcore && module->techtype != tt_lifesupport){
                            module->powerDraw--;
                            if(module->powerDraw < 1) module->powerDraw++;
                        }
                    }
                    break;
            }
        }
        keys_prior[k_Left] = key;
      
        if(player->tick % POWER_INTERVAL == 0){
            PROC_PowerDraw(&ShipModules, player->moduleRepairing);
            PROC_PowerCycle(&ShipModules, warpcore, player->moduleRepairing, &player);
        }
        
        if(player->tick % REPAIR_INTERVAL == 0){
            if(player->moduleRepairing != -1){
                Module_t* repair = &ShipModules[player->moduleRepairing];
                if(!repair->online && repair->health < repair->maxHealth && repair->powerReserve)
                    repair->health++;
                if(repair->health * 100 / repair->maxHealth >= 50){
                    if(repair->location){
                        switch(repair->location){
                            case nacelles:
                                player->damagesection[nacelles-1] = false;
                                break;
                            case aft:
                                player->damagesection[aft-1] = false;
                                break;
                            case saucer:
                                player->damagesection[saucer-1] = false;
                                break;
                        }
                    }
                }
            }
        }
        
        
        switch(player->ScreenSelected){
            case SCRN_VIEW:
                gfx_WipeScreen();
                gfx_SetClipRegion(23, 22, 297, 186);
                if(speed >= 10) GUI_RenderWarp(speed);
                gfx_SetClipRegion(0, 0, 320, 240);
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, shields, activeweap);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules, player->moduleSelected, player->moduleRepairing);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules, player->moduleSelected, player->position.speed);
                break;
        }
        
       
        
       
        
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
        gfx_DrawShipStatusIcon(integrity, shields, &player);
        gfx_DrawSpeedIndicator(player->position.speed, topspeed);
        gfx_BlitBuffer();
        player->tick++;
    }
    while(loopgame);
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
        case 3:
            gfx_PrintStringXY("An internal error occured!", 1, 0);
            break;
        case 4:
            gfx_PrintStringXY("Failed to return pointers!", 1, 0);
            break;
    }
    gfx_PrintStringXY("Thanks for testing!!", 1, 10);
    free(MapMain);
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


