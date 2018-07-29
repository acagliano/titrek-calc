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



/* Put your function prototypes here */
void printText(const char *text, uint8_t x, uint8_t y);

#include "gfx_functions.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "processes.h"
#include "initialize.h"

#include "screens.h"

#include "equates.h"

Player_t player[1] = {0};
Module_t ShipModules[20] = {0};
Module_t *shields = NULL, *integrity = NULL, *auxpower = NULL, *lifesupport = NULL, *warpcore = NULL;
const char *GameSave = "TrekSave";

void main(void) {
    char SavePtr;
    char i;
    char key = 0;
    malloc(0);
    ti_CloseAll();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(1);
    gfx_SetTransparentColor(0);
	/* Fill in the body of the main function here */
    for(i = 0; i <= 8; i++){
        Module_t *module = &ShipModules[i];
        module->online = true;
        module->techtype = i;
        module->health = 50;
        module->maxHealth = 50;
        module->powerReserve = 128;
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
            case tt_comms:
                module->modtype = mt_system;
                strcpy(module->techname, "Comms");
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
    boot_ClearVRAM();
    DrawGUI(&ShipModules);
    player->ScreenSelected = SCRN_POWER;
    player->timers[timer_power] = POWER_INTERVAL;

    do
    {
        key = os_GetCSC();
       
        if(key == sk_Clear) break;
       
        if( key == sk_Down){
            Module_t* module;
            switch(player->ScreenSelected){
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
                case SCRN_POWER:
                    module = &ShipModules[player->moduleSelected];
                    if(module->techtype != tt_warpcore){
                        module->powerDraw--;
                        if(module->powerDraw < 1) module->powerDraw++;
                    }
                    break;
            }
        }
        
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
                if(player->moduleRepairing == player->moduleSelected + 1) player->moduleRepairing = 0;
                else player->moduleRepairing = player->moduleSelected + 1;
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
            char int_drv = integrity->health * 100 / integrity->maxHealth;
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
                DrawGUI();
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, shields);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules, player->moduleSelected, player->moduleRepairing);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules, player->moduleSelected);
                break;
        }
        if(player->timers[timer_power] == 0){
            PROC_PowerDraw(&ShipModules, player->moduleRepairing);
            PROC_PowerCycle(&ShipModules, player->moduleRepairing);
            player->timers[timer_power] = POWER_INTERVAL;
        }
        else player->timers[timer_power]--;
        
        if(player->timers[timer_repair] == 0){
            if(player->moduleRepairing){
                Module_t* repair = &ShipModules[player->moduleRepairing - 1];
                if(!repair->online && repair->health < repair->maxHealth && repair->powerReserve)
                    repair->health++;
                player->timers[timer_repair] = REPAIR_INTERVAL;
            }
        }
        else player->timers[timer_repair]--;
        
        gfx_DrawShipStatusIcon(integrity, shields);
        gfx_DrawInventoryStatusIcon(true);
        gfx_BlitBuffer();
        
        if(!warpcore->health || !warpcore->online){
            player->timers[timer_corebreach]++;
            if(player->timers[timer_corebreach] == CORE_BREACH_TIMER){
                player->deathreason = 1;
                break;
            }
        } else
            player->timers[timer_corebreach] = 0;
        
        if(!lifesupport->health || !lifesupport->online){
            player->timers[timer_lifesupport]++;
            if(player->timers[timer_lifesupport] == LIFE_SUPPORT_TIMER){
                player->deathreason = 2;
                break;
            }
        } else
            player->timers[timer_lifesupport] = 0;
        
    }
    while(key != sk_Clear);
    gfx_SetDrawScreen();
    boot_ClearVRAM();
    
    
    // Cleanup
    ti_CloseAll();
    switch(player->deathreason){
        case 1:
            gfx_PrintStringXY("Warp Core has breached!", 1, 0);
            break;
        case 2:
            gfx_PrintStringXY("Life Support has failed!", 1, 0);
            break;
    }
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


