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
char gfx_MainMenu(bool gfx_initialized);

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
#include "gfx/trekvfx.h"
#include "gfx/trekicon.h"

#include "screens.h"

#include "equates.h"

typedef struct {
    bool impact;
    char damage;
    char bleedthrough;
    char internal;
} damage_desc_t;

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
Module_t ShipModules[15] = {0};
MapData_t* MapMain;
Module_t *shields = NULL, *integrity = NULL, *auxpower = NULL;
Module_t  *lifesupport = NULL, *warpcore = NULL;
Module_t *warpdrive = NULL, *impulsedrive = NULL, *activeweapon = NULL;
const char *GameSave = "TrekSave";

void main(void) {
    bool loopgame = true, key = false;
    char looplimit = sizeof(ShipModules) / sizeof(Module_t);
    bool gfx_initialized[2];
    animation_t animation[2] = {0};
    animation_t *temp;
    gfx_sprite_t* uncompressed;
    long tick_test;
    bool keys_prior[15] = {0};
    char SavePtr;
    char i = 0, j;
    char mapslot;
    srandom(rtc_Time());
    ti_CloseAll();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_initialized[0] = trekicon_init();
    gfx_initialized[1] = trekvfx_init();
    temp = &animation[0];
    temp->origin_x = 11;
    temp->origin_y = 25;
    temp->duration = 0;
    temp = &animation[1];
    temp->origin_x = 319;
    temp->origin_y = 25;
    temp->duration = 0;
    while(i != 1){
        i = gfx_MainMenu(gfx_initialized[0]);
        if(i == 3) {
            gfx_End();
            prgm_CleanUp();
            return;
        }
        if(i == 2){
            gfx_ZeroScreen();
            gfx_SetTextFGColor(231);
            gfx_SetTextBGColor(255);
            gfx_PrintStringXY("Star Trek Multiplayer, ", 40, 1);
            gfx_PrintString(trek_version);
            gfx_Line(0, 12, 320, 12);
            gfx_PrintStringXY("ACagliano - concept, general programming", 5, 15);
            gfx_PrintStringXY("GregsAStar - 3d trig, rendering", 5, 25);
            gfx_PrintStringXY("Zeda - framebuffer sorting", 5, 35);
            gfx_PrintStringXY("TurquoiseDragon - spritework", 5, 45);
            gfx_PrintStringXY("Press any key to continue.", 5, 230);
            gfx_BlitBuffer();
            while(!os_GetCSC());
        }
    }
    gfx_SetTextFGColor(255);
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
        module->pdConstant = true;
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
                module->pdConstant = false;
                strcpy(module->techname, "WarpCore");
                module->powerReserve = 256;
                module->stats.sysstats.powerOut = 50;
                module->online = true;
                break;
            case tt_warpdrive:
                module->location = nacelles;
                module->pdConstant = false;
                module->stats.sysstats.topSpeed = 11;
                strcpy(module->techname, "WarpDrive");
                break;
            case tt_impulsedrive:
                module->location = aft;
                module->pdConstant = false;
                module->stats.sysstats.topSpeed = 4;
                module->powerDraw = 4;
                module->powerDefault = 4;
                strcpy(module->techname, "ImpulseDr");
                break;
            case tt_transporter:
                module->location = saucer;
                module->pdConstant = false;
                strcpy(module->techname, "Transport");
                break;
            case tt_sensor:
                module->location = saucer;
                module->pdConstant = false;
                strcpy(module->techname, "Sensors");
                break;
            case tt_phaser:
                module->location = saucer;
                module->powerDraw = 10;
                module->powerDefault = 10;
                module->pdConstant = false;
                module->stats.weapstats.charge = 0;
                module->stats.weapstats.maxCharge = 10;
                module->stats.weapstats.damage_shield = 2;
                module->stats.weapstats.damage_hull = 1;
                module->stats.weapstats.range = 100;
                module->stats.weapstats.speed = 10;
                strcpy(module->techname, "Phasers");
                strcpy(module->stats.weapstats.weapname, "Pulse");
                break;
            case tt_torpedo:
                module->location = saucer;
                module->pdConstant = false;
                module->stats.weapstats.damage_shield = 1;
                module->stats.weapstats.damage_hull = 3;
                module->stats.weapstats.range = 100;
                module->stats.weapstats.speed = 5;
                strcpy(module->techname, "Torpedo");
                strcpy(module->stats.weapstats.weapname, "Photon");
                break;
            default:
                strcpy(module->techname, "Unsupport");
        }
    }
    shields = init_SetPointer(&ShipModules, looplimit, tt_shield, 0);
    integrity = init_SetPointer(&ShipModules, looplimit, tt_integrity, 0);
    auxpower = init_SetPointer(&ShipModules, looplimit, tt_auxiliary, 0);
    warpcore = init_SetPointer(&ShipModules, looplimit, tt_warpcore, 0);
    impulsedrive = init_SetPointer(&ShipModules, looplimit, tt_impulsedrive, 0);
    lifesupport = init_SetPointer(&ShipModules, looplimit, tt_lifesupport, 0);
    warpdrive = init_SetPointer(&ShipModules, looplimit, tt_warpdrive, 0);
    activeweapon = init_SetPointer(&ShipModules, looplimit, tt_phaser, 0);
    MapMain = calloc(10, sizeof(MapData_t));
    boot_ClearVRAM();
    DrawGUI();
    player->ScreenSelected = SCRN_VIEW;
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
        damage_desc_t shiphit = {0};
        char corestability, lifesupporthealth, speed = player->position.speed;
        char integrityhealth;
        unsigned char warpdrivehealth = warpdrive->health * 100 / warpdrive->maxHealth;
        unsigned char impdrivehealth = impulsedrive->health * 100 / impulsedrive->maxHealth;
        char topspeed_warp = warpdrive->stats.sysstats.topSpeed;
        char topspeed_impulse = impulsedrive->stats.sysstats.topSpeed;
        char topspeed;
        unsigned char warppow = warpdrive->powerDraw * 100 / warpdrive->powerDefault;
        unsigned char imppow = impulsedrive->powerDraw * 100 / impulsedrive->powerDefault;
        if( warppow > 100) topspeed_warp += (warppow / 10 - 10) / 2;
        else topspeed_warp = topspeed_warp * warppow / 100;
        if( imppow > 100 ) topspeed_impulse += (imppow / 100);
        else topspeed_impulse = topspeed_impulse * imppow / 100;
        topspeed_warp = topspeed_warp * warpdrivehealth / 100;
        topspeed_impulse = topspeed_impulse * impdrivehealth / 100;
        if(topspeed_warp < 10) topspeed_warp = 10;
        if(topspeed_impulse < 2) topspeed_impulse = 2;
        topspeed = topspeed_warp;
        if(warpdrivehealth == 0 || warppow == 0 || !warpdrive->online) topspeed = topspeed_impulse;
        if(speed < 10 && (impdrivehealth == 0 || imppow == 0 || !impulsedrive->online)) topspeed = 0;
        if(speed > topspeed) {
            player->position.speed = topspeed;
            speed = topspeed;
        }
        
        kb_ScanGroup(1);
        key = kb_Data[1] & kb_Window;
        if(key && !keys_prior[k_Window] ){
            if(player->ScreenSelected == SCRN_STATUS) player->ScreenSelected = (player->viewScreenPrior) ? SCRN_VIEW_TARG : SCRN_VIEW;
            else player->ScreenSelected = SCRN_STATUS;
        }
        keys_prior[k_Window] = key;
        key = kb_Data[1] & kb_Trace;
        if( key && !keys_prior[k_Trace] ){
            if(player->ScreenSelected == SCRN_POWER) player->ScreenSelected = (player->viewScreenPrior) ? SCRN_VIEW_TARG : SCRN_VIEW;
            else player->ScreenSelected = SCRN_POWER;
        }
        keys_prior[k_Trace] = key;
        key = kb_Data[1] & kb_Yequ;
        if( key && !keys_prior[k_Yequ] ){
            if(player->ScreenSelected == SCRN_TACTICAL) player->ScreenSelected = (player->viewScreenPrior) ? SCRN_VIEW_TARG : SCRN_VIEW;
            else player->ScreenSelected = SCRN_TACTICAL;
        }
        keys_prior[k_Yequ] = key;
        
        key = kb_Data[1] & kb_Mode;
        if( key && !keys_prior[k_Mode] ){
            if(player->ScreenSelected == SCRN_VIEW) {
                player->ScreenSelected = SCRN_VIEW_TARG;
                player->viewScreenPrior = SCRN_VIEW_TARG;
            }
            else if(player->ScreenSelected == SCRN_VIEW_TARG) {
                player->ScreenSelected = SCRN_VIEW;
                player->viewScreenPrior = 0;
            }
            if(player->ScreenSelected == SCRN_POWER){
                Module_t* module = &ShipModules[player->moduleSelected];
                char health = module->health * 100 / module->maxHealth;
                char power = module->powerReserve * 100 / 256;
                module->online = !module->online;
                if(health < 25 || power < 25) module->online = false;
                    
            }
            if(player->ScreenSelected == SCRN_TACTICAL){
                char i = (activeweapon->techtype == tt_phaser) ? tt_torpedo : tt_phaser;
                activeweapon = init_SetPointer(&ShipModules, looplimit, i, 0);
            }

            if(player->ScreenSelected == SCRN_STATUS){
                Module_t* module = &ShipModules[player->moduleSelected];
                char modhealth = module->health * 100 / module->maxHealth;
                if(player->moduleRepairing == player->moduleSelected){
                    player->moduleRepairing = -1;
                    if(modhealth > 25) module->online = true;
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
        if( key && (!keys_prior[k_2nd] || player->tick % 2 == 0) ){
            if(activeweapon && activeweapon->online){
                int power = activeweapon->powerReserve;
                char charge = activeweapon->stats.weapstats.charge;
                char maxCharge = activeweapon->stats.weapstats.maxCharge;
                if((power >= ((charge + 1) * activeweapon->powerDraw)) && (charge < maxCharge)){
                    activeweapon->stats.weapstats.charge++;
                }
            }
        }
        if(!key && keys_prior[k_2nd]){
            if(activeweapon){ //fire phaser
                activeweapon->powerReserve -= (activeweapon->stats.weapstats.charge * activeweapon->powerDraw);
                activeweapon->stats.weapstats.charge = 0;
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
        if( (key && (!keys_prior[k_Add] || player->tick % 5 == 0))){
            if(speed < 10 && speed < topspeed_impulse) player->position.speed++;
            if(speed >= 10 && speed < topspeed_warp ) player->position.speed++;
        }
        keys_prior[k_Add] = key;
        key = kb_Data[6] & kb_Sub;
        if( (key && (!keys_prior[k_Sub] || player->tick % 5 == 0)) && speed > 0)
            if(speed > 10 || speed <= 9) player->position.speed--;
        keys_prior[k_Sub] = key;
        
        if( kb_Data[6] & kb_Power){
            //damage random system
            char i;
            char damage = randInt(4,6);
            char drv = shields->stats.shieldstats.resistance * shields->powerDraw * shields->health / shields->powerDefault / shields->maxHealth;
            char int_drv = integrity->health * integrity->powerDraw * 100 / integrity->maxHealth / integrity->powerDefault;
            shiphit.impact = true;
            shiphit.damage = damage;
            if(shields->online){
                shields->health -= damage;
                if(shields->health < 0) shields->health = 0;
                damage -= drv;
                if(damage < 0) damage = 0;
            }
            shiphit.bleedthrough = damage;
            drv = 0;
            if(integrity->online){
                drv = integrity->stats.sysstats.resistance;
                if(int_drv < 25) drv = drv>>1;
                if(int_drv == 0) drv = 0;
                integrity->health -= damage;
                if(integrity->health < 0) integrity->health = 0;
                damage -= drv;
                if(damage < 0) damage = 0;
            }
            shiphit.internal = damage;
            for(i = 0; i < 2; i++){
                Module_t* module = &ShipModules[randInt(tt_lifesupport-1,tt_warpcore-1)];
                module->health -= damage;
                if(module->health <= 0) {
                    module->health = 0;
                    module->online = false;
                }
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
                case SCRN_VIEW_TARG:
                    if(player->target.angles[1] < 40) player->target.angles[1]++;
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
                case SCRN_VIEW_TARG:
                    if(player->target.angles[1] > -40) player->target.angles[1]--;
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
                case SCRN_VIEW_TARG:
                    if(player->target.angles[0] < 40) player->target.angles[0]++;
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
                case SCRN_VIEW_TARG:
                    if(player->target.angles[0] > -40) player->target.angles[0]--;
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
            if(speed < 10 && speed > 0){
                impulsedrive->powerReserve -= speed;
                if(impulsedrive->powerReserve <= 0) {
                    impulsedrive->powerReserve = 0;
                    impulsedrive->online = false;
                }
            }
            if(speed >= 10){
                warpdrive->powerReserve -= (speed / 2);
                if(warpdrive->powerReserve <= 0) {
                    warpdrive->powerReserve = 0;
                    warpdrive->online = false;
                }
            }
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
        
        if(warpcore && player->tick % CORE_RECHARGE_TIMER == 0)
            if(warpcore->powerReserve < 256) warpcore->powerReserve++;
        gfx_WipeScreen();
        
        gfx_SetClipRegion(xStart, yStart, xStart + vWidth, yStart + vHeight);
        switch(player->ScreenSelected){
            case SCRN_VIEW_TARG:
            case SCRN_VIEW:
                if(shiphit.impact) {
                    int j, damage = shiphit.damage;
                    if(shields->online && shields->health * 100 / shields->maxHealth) gfx_SetColor(223);
                    else gfx_SetColor(255);
                    gfx_FillRectangle(xStart, yStart, vWidth, vHeight);
                    if(shiphit.bleedthrough){
                        for(j = 34; j < vHeight + 21; j++)
                            if(randInt(shiphit.bleedthrough, damage + 5) == damage){
                                gfx_SetColor(randInt(0,255));
                                gfx_HorizLine(xStart+1, j, vWidth - 1);
                            }
                    }
                } else {
                    if(speed >= 10) GUI_RenderWarp(speed);
                }
                // render frame buffer
                if(player->ScreenSelected == SCRN_VIEW || player->ScreenSelected == SCRN_VIEW_TARG){
                    // vector display
                    unsigned char anglexz = player->position.angles[0];
                    unsigned char angley = player->position.angles[1];
                    int coords;
                    int dialx = xStart+17;
                    unsigned char dialy = yStart+vHeight-25;
                    char vectorx = 10 * byteSin(anglexz) / 127;
                    char vectory = -10 * byteCos(anglexz) / 127;
                    gfx_SetColor(255);
                    gfx_SetTextXY(xStart + 4, dialy - 18);
                    gfx_PrintUInt(anglexz,3);
                    gfx_Circle(dialx, dialy + 1, 10);
                    gfx_Line(xStart + 17, dialy+1, xStart + 17 + vectorx, dialy + 1 + vectory);
                    gfx_PrintStringXY("Rot", xStart+4, dialy + 15);
                    dialx += 25;
                    vectorx = 10 * byteCos(angley) / 127;
                    vectory = 10 * byteSin(angley) / 127;
                    gfx_SetTextXY(dialx - 9, dialy - 18);
                    gfx_PrintUInt(angley,3);
                    gfx_Circle(dialx, dialy+1, 10);
                    gfx_Line(dialx, dialy+1, dialx + vectorx, dialy+1 + vectory);
                    gfx_PrintStringXY("Pit", dialx - 9, dialy + 15);
                    gfx_Rectangle(xStart + vWidth - 85, yStart+vHeight-23, 85, 23);
                    gfx_PrintStringXY("Sector:", xStart+vWidth-82, yStart+vHeight-20);
                    gfx_SetTextXY(xStart+vWidth-82, yStart+vHeight-10);
                    gfx_PrintUInt(player->position.coords[0]>>16, 3);
                    gfx_PrintString(":");
                    gfx_PrintUInt(player->position.coords[1]>>16, 3);
                    gfx_PrintString(":");
                    gfx_PrintUInt(player->position.coords[2]>>16, 3);
                }
                if(player->ScreenSelected == SCRN_VIEW_TARG){
                    gfx_sprite_t *uncompressed;
                    int view_center_x = vWidth / 2;
                    int view_center_y = vHeight / 2;
                    char anglexz = player->target.angles[0];
                    char angley = player->target.angles[1];
                    gfx_SetTextFGColor(224);
                    gfx_SetTextXY(xStart + vWidth - 84, yStart + vHeight - 40);
                    if(activeweapon->techtype == tt_phaser) gfx_PrintString("[ Phasers ]");
                    if(activeweapon->techtype == tt_torpedo) gfx_PrintString("[ Torpedo ]");
                    gfx_SetTextFGColor(255);
                    if(abs(anglexz) < 45 && abs(angley) < 45){
                        char charge = activeweapon->stats.weapstats.charge;
                        char maxCharge = activeweapon->stats.weapstats.maxCharge;
                        int targ_gui_x = view_center_x * anglexz / 45 + view_center_x;
                        int targ_gui_y = view_center_y * angley / 45 + view_center_y;
                        gfx_SetColor(224);
                        if(uncompressed = gfx_MallocSprite(target_width, target_height)){
                            zx7_Decompress(uncompressed, target_compressed);
                            gfx_TransparentSprite(uncompressed,
                                                  xStart + targ_gui_x - (target_width>>1),
                                                  yStart + targ_gui_y - (target_height>>1));
                            free(uncompressed);
                        }
                       /* gfx_Circle(xStart + targ_gui_x, yStart + targ_gui_y, 15);
                        gfx_Circle(xStart + targ_gui_x, yStart + targ_gui_y, 8); */
                        gfx_HorizLine(xStart + targ_gui_x - 15, yStart + targ_gui_y, 30);
                        gfx_VertLine(xStart + targ_gui_x, yStart + targ_gui_y - 15, 30);
                        gfx_FillCircle(xStart + targ_gui_x, yStart + targ_gui_y, charge * 8 / maxCharge);
                    }
                }
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, shields, activeweapon);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules, looplimit, player->moduleSelected, player->moduleRepairing);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules, looplimit, player->moduleSelected, player->position.speed);
                break;
        }
        gfx_SetClipRegion(0, 0, 320, 240);
        
        
        if(warpcore){
            corestability = warpcore->health * 100 / warpcore->maxHealth;
            lifesupporthealth = lifesupport->health * 100 / lifesupport->maxHealth;
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
        else if(lifesupporthealth > 25 && lifesupport->online) player->timers[timer_lifesupport] = 0;
        if(player->timers[timer_lifesupport]) player->timers[timer_lifesupport]++;
        if(player->timers[timer_lifesupport] == LIFE_SUPPORT_TIMER){
            player->deathreason = 2;
            break;
        }
        gfx_SetColor(148); gfx_FillRectangle(225, 203, 20 * 3, 20);
        if(player->timers[timer_lifesupport]) gfx_DrawLifeSupportAlert();
        if(player->timers[timer_corebreach]) gfx_DrawCoreBreachAlert();
        gfx_DrawShipStatusIcon(integrity, shields, &player);
        gfx_DrawSpeedIndicator(player->position.speed, topspeed_warp, topspeed_impulse);
        integrityhealth = integrity->health * 100 / integrity->maxHealth;
        if(integrityhealth < 50){
          
            
        }
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

char gfx_MainMenu(bool gfx_initialized){
    bool splash_first_loop = true;
    gfx_sprite_t *uncompressed;
    char key = 0, i;
    gfx_ZeroScreen();
    if(gfx_initialized){
        if(uncompressed = gfx_MallocSprite(75,116)){
            zx7_Decompress(uncompressed, splashlogo_compressed);
            gfx_Sprite(uncompressed, 20, 50);
            free(uncompressed);
        }
    }
    gfx_SetTextScale(3,3);
    gfx_SetTextFGColor(26);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(0);
    gfx_PrintStringXY("Star Trek", 103, 53);
    gfx_SetTextFGColor(230);
    gfx_PrintStringXY("Star Trek", 100, 50);
    gfx_SetTextScale(2,2);
    gfx_PrintStringXY("Multiplayer", 120, 80);
    gfx_SetColor(230);
    gfx_FillRectangle(100, 100, 200, 2);
    gfx_SetColor(231);
    gfx_FillRectangle(140, 110, 100, 45);
    gfx_SetTextScale(1,1);
    gfx_PrintStringXY("Boldly going where no", 10, 220);
    gfx_PrintStringXY("calculator has gone before!", 10, 230);
    gfx_PrintStringXY(trek_version, 240, 230);
    gfx_SetTextTransparentColor(255);
    gfx_SetTextFGColor(0);
    gfx_SetTextBGColor(255);
    i = 0;
    do {
        key = os_GetCSC();
        if(key == sk_Up) i--;
        if(key == sk_Down) i++;
        if(key == sk_Clear) {
            key = sk_Enter;
            i = 2;
        }
        if(i < 0) i = 2;
        if(i > 2) i = 0;
        if(key || splash_first_loop) {
            gfx_SetColor(231);
            gfx_FillRectangle(140, 110, 100, 45);
            gfx_SetColor(229);
            gfx_FillRectangle(140, i * 15 + 110, 100, 15);
            gfx_PrintStringXY("Start Demo", 150, 114);
            gfx_PrintStringXY("Credits", 150, 129);
            gfx_PrintStringXY("Exit Game", 150, 144);
            splash_first_loop = false;
        }
        gfx_BlitBuffer();
    } while (key != sk_Enter);
    return i + 1;
}

