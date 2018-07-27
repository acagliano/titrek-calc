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
Module_t *shields = NULL, *integrity = NULL, *auxpower = NULL;
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
        module->powerReserve = 0;
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
    boot_ClearVRAM();
    DrawGUI(&ShipModules);
    player->ScreenSelected = SCRN_POWER;
    player->powFreq = 5;

    do
    {
        key = os_GetCSC();
       
        if(key == sk_Clear) break;
       
        if( key == sk_Yequ ){
            if(player->ScreenSelected == SCRN_TACTICAL) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_TACTICAL;
        }
        
        if( key == sk_Window ){
            if(player->ScreenSelected == SCRN_STATUS) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_STATUS;
        }
        
        if( key == sk_Trace ){
            if(player->ScreenSelected == SCRN_POWER) player->ScreenSelected = SCRN_VIEW;
            else player->ScreenSelected = SCRN_POWER;
        }
        
     /*   if( key == sk_Power){
            //damage random system
            char i;
            char damage = randInt(5,8);
            char drv = shields->stats.shieldstats.resistance * shields->powerDraw * shields->health / shields->powerDefault / shields->maxHealth;
            char int_drv = integrity->health * 100 / integrity->maxHealth;
            shields->health -= damage;
            damage -= drv;
            if(int_drv >= 50) drv = integrity->stats.sysstats.resistance;
            if(int_drv < 50) drv = 0;
            if(!int_drv) drv = 0 - integrity->stats.sysstats.resistance;
            integrity->health -= damage;
            damage -= drv;
            if(damage < 0) damage = 0;
            for(i = 0; i < 2; i++){
                Module_t* module = &ShipModules[randInt(2,12)];
                module->health -= damage;
            }
        }*/
        
        
        switch(player->ScreenSelected){
            case SCRN_VIEW:
                DrawGUI();
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, shields);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules);
                break;
        }
        if(player->powFreq == 0){
            PROC_PowerDraw(&ShipModules);
            PROC_PowerCycle(&ShipModules);
            player->powFreq = 5;
        }
        else player->powFreq--;
        GUI_UpdateIcons(&ShipModules);
    }
    while(key != sk_Clear);
    gfx_SetDrawScreen();
    boot_ClearVRAM();
    
    
    // Cleanup
    ti_CloseAll();
    
    gfx_PrintStringXY("Modules Removed", 1, 0);
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


