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


#include "render.h"
/* Put your function prototypes here */
void printText(const char *text, uint8_t x, uint8_t y);
char gfx_MainMenu(bool gfx_initialized, buffers_t* buffers);

#include "gfx_functions.h"
#include "datatypes/mapdata.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "processes.h"
#include "initialize.h"
#include "mapfuncs.h"
#include "mymath.h"
#include "vfx.h"
#include "gfx/trekgui.h"
#include "gfx/trekvfx.h"
#define gui_enabled 0
#define vfx_enabled 1

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
    k_2nd,
    k_Alpha,
    k_GraphVar,
    k_Enter
};
Player_t player[1] = {0};
Module_t ShipModules[15] = {0};
MapData_t MapMain[20] = {0};
renderitem_t renderbuffer[20] = {0};
Module_t *activeweapon = NULL;
gfx_sprite_t *sprites[trekvfx_num];
const char *GameSave = "TrekSave";
#define shields ((Module_t*)&ShipModules[tt_shield-1])
#define integrity ((Module_t*)&ShipModules[tt_integrity-1])
#define lifesupport ((Module_t*)&ShipModules[tt_lifesupport-1])
#define warpdrive ((Module_t*)&ShipModules[tt_warpdrive-1])
#define impulsedrive ((Module_t*)&ShipModules[tt_impulsedrive-1])
#define phaser ((Module_t*)&ShipModules[tt_phaser-1])
#define torpedo ((Module_t*)&ShipModules[tt_torpedo-1])
#define sensors ((Module_t*)&ShipModules[tt_sensor-1])
#define transporter ((Module_t*)&ShipModules[tt_transporter-1])
#define auxpower ((Module_t*)&ShipModules[tt_auxiliary-1])
#define warpcore ((Module_t*)&ShipModules[tt_warpcore-1])

void main(void) {
    bool loopgame = true, key = false;
    char looplimit = sizeof(ShipModules) / sizeof(Module_t);
    bool gfx_initialized[2];
    animation_t animation[2] = {0};
    animation_t *temp;
    gfx_sprite_t* uncompressed;
    buffers_t buffers;
    long tick_test;
    bool keys_prior[18] = {0};
    char SavePtr;
    unsigned char i = 0, j;
    char mapslot;
    buffers.uncompressed = gfx_MallocSprite(64, 64);
    buffers.rotated = gfx_MallocSprite(64, 64);
    buffers.scaled = gfx_MallocSprite(128, 128);
    srandom(rtc_Time());
    ti_CloseAll();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTextBGColor(32);
    gfx_SetTextTransparentColor(32);
    gfx_initialized[gui_enabled] = trekgui_init();
    gfx_initialized[vfx_enabled] = trekvfx_init();
    gfxinit_DecompressAll(&sprites);
    if(!gfx_initialized[gui_enabled] || !gfx_initialized[vfx_enabled]){
        player->deathreason = 4;
        loopgame = false;
    }
    temp = &animation[0];
    temp->origin_x = 11;
    temp->origin_y = 25;
    temp->duration = 0;
    temp = &animation[1];
    temp->origin_x = 319;
    temp->origin_y = 25;
    temp->duration = 0;
    while(i != 1){
        i = gfx_MainMenu(gfx_initialized[0], &buffers);
        if(i == 3) {
            gfx_End();
            prgm_CleanUp();
            return;
        }
        if(i == 2){
            gfx_ZeroScreen();
            gfx_SetTextFGColor(231);
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
	/* Fill in the body of the main function here */
    for(i = 1; i <= 11; i++){
        Module_t *module;
        if(i <= 9) module = &ShipModules[i-1];
        else {
            if(i == 10) module = &ShipModules[tt_warpcore-1];
            if(i == 11) module = &ShipModules[tt_auxiliary-1];
        }
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
                module->stats.sysstats.sensor_range = 300;
                module->pdConstant = true;
                strcpy(module->techname, "Sensors");
                break;
            case tt_phaser:
                module->location = saucer;
                module->powerDraw = 10;
                module->powerDefault = 10;
                module->pdConstant = false;
                module->stats.weapstats.charge = 0;
                module->stats.weapstats.maxCharge = 5;
                module->stats.weapstats.damage_shield = 2;
                module->stats.weapstats.damage_hull = 1;
                module->stats.weapstats.range = 25;
                module->stats.weapstats.speed = 20;
                strcpy(module->techname, "Phasers");
                strcpy(module->stats.weapstats.weapname, "Pulse");
                break;
            case tt_torpedo:
                module->location = saucer;
                module->pdConstant = false;
                module->stats.weapstats.damage_shield = 1;
                module->stats.weapstats.damage_hull = 3;
                module->stats.weapstats.range = 50;
                module->stats.weapstats.speed = 10;
                strcpy(module->techname, "Torpedo");
                strcpy(module->stats.weapstats.weapname, "Photon");
                break;
            case 10:
                module->techtype = tt_warpcore;
                module->pdConstant = false;
                strcpy(module->techname, "WarpCore");
                module->powerReserve = 256;
                module->stats.sysstats.powerOut = 50;
                break;
            case 11:
                module->techtype = tt_auxiliary;
                module->pdConstant = false;
                strcpy(module->techname, "AuxPower");
                module->powerReserve = 0;
                module->stats.sysstats.auxiliarymax = 5000;
                break;
            default:
                strcpy(module->techname, "Unsupport");
        }
    }
    
    
    
    activeweapon = phaser;
    boot_ClearVRAM();
    DrawGUI();
    player->ScreenSelected = SCRN_VIEW;
    AnglesToVectors(&player->position);
    player->moduleRepairing = -1;
    if((mapslot = map_LocateSlot(&MapMain)) != -1){
        MapData_t *slot = &MapMain[mapslot];
        // assign ship data to slot
        slot->entitytype = et_ship;
        slot->mobile = false;
        slot->entitystats.ship.health = 50;
        slot->entitystats.ship.maxHealth = 50;
        slot->entitystats.ship.shield = 50;
        slot->entitystats.ship.shieldMax = 50;
        slot->position.coords.x = randInt(100,150)<<8;
        slot->position.coords.y = randInt(100,150)<<8;
        slot->position.coords.z = randInt(100,150)<<8;
    }
    do
    {
        damage_desc_t shiphit = {0};
        char corestability, lifesupporthealth, speed;
        char map_count = GUI_PrepareFrame(&MapMain, &renderbuffer, &player->position);
        char integrityhealth;
        char moduleselected = player->moduleSelected;
        char modulerepairing = player->moduleRepairing;
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
        if(warpdrivehealth < 50){
            topspeed_warp = topspeed_warp * warpdrivehealth / 50;
            player->position.topspeed.warp = topspeed_warp;
        }
        if(impdrivehealth < 50){
            topspeed_impulse = topspeed_impulse * impdrivehealth / 50;
            player->position.topspeed.impulse = topspeed_impulse;
        }
        if(speed < 10 && (impdrivehealth == 0 || !impulsedrive->online)) topspeed_impulse = 0;
        if(topspeed_warp < 10 || warpdrivehealth == 0 || !warpdrive->online)
            player->position.warpactive = false;
        if(player->position.warpactive) {
            speed = player->position.speed.warp;
            if(speed > topspeed_warp) {
                speed = topspeed_warp;
                player->position.speed.warp = topspeed_warp;
            }
        } else {
            speed = player->position.speed.impulse;
            if(speed > topspeed_impulse){
                speed = topspeed_impulse;
                player->position.speed.impulse = topspeed_impulse;
            }
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
        key = kb_Data[1] & kb_Zoom;
        if( key && !keys_prior[k_Zoom] ){
            if(player->ScreenSelected == SCRN_SENSORS) player->ScreenSelected = (player->viewScreenPrior) ? SCRN_VIEW_TARG : SCRN_VIEW;
            else player->ScreenSelected = SCRN_SENSORS;
        }
        keys_prior[k_Zoom] = key;
        key = kb_Data[1] & kb_Yequ;
        if( key && !keys_prior[k_Yequ] ){
            if(player->ScreenSelected == SCRN_TACTICAL) player->ScreenSelected = (player->viewScreenPrior) ? SCRN_VIEW_TARG : SCRN_VIEW;
            else player->ScreenSelected = SCRN_TACTICAL;
        }
        keys_prior[k_Yequ] = key;
        
        key = kb_Data[1] & kb_Mode;
        if( key && !keys_prior[k_Mode] ){
            if(player->ScreenSelected == SCRN_TACTICAL)
                shields->online = !shields->online;
            if(player->ScreenSelected == SCRN_STATUS){
                Module_t* module = &ShipModules[moduleselected];
                char health = module->health * 100 / module->maxHealth;
                char power = module->powerReserve * 100 / 256;
                module->online = !module->online;
                if(health < 25 || power < 25) module->online = false;
                
            }
            if(player->ScreenSelected == SCRN_VIEW) {
                player->ScreenSelected = SCRN_VIEW_TARG;
                player->viewScreenPrior = SCRN_VIEW_TARG;
            }
            else if(player->ScreenSelected == SCRN_VIEW_TARG) {
                player->ScreenSelected = SCRN_VIEW;
                player->viewScreenPrior = 0;
            }
            if(player->ScreenSelected == SCRN_POWER){
                player->powersource = !player->powersource;
            }
        }
        keys_prior[k_Mode] = key;
        
        key = kb_Data[1] & kb_Del;
        if( key && !keys_prior[k_Del] ){
            Module_t *module = &ShipModules[moduleselected];
            if(module->techtype == tt_warpcore && player->timers[timer_corebreach]){
                module->techtype = tt_ejectedcore;
                player->timers[timer_corebreach] = 0;
            }
        }
        keys_prior[k_Del] = key;
        
        key = kb_Data[1] & kb_2nd;
        if( key && activeweapon){
            if(activeweapon->online){
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
                if(activeweapon){
                    char index;
                    activeweapon->powerReserve -= (activeweapon->stats.weapstats.charge * activeweapon->powerDraw);
                    if((index = map_LocateSlot(&MapMain)) != -1){
                        MapData_t *slot = &MapMain[index];
                        // copy and offset position
                        Position_t *playerpos = &player->position;
                        Position_t *entitypos = &slot->position;
                        entitypos->coords.x = playerpos->coords.x;
                        entitypos->coords.y = playerpos->coords.y;
                        entitypos->coords.z = playerpos->coords.z;
                        entitypos->angles.xz = playerpos->angles.xz;
                        entitypos->angles.y = playerpos->angles.y;
                        slot->speed = activeweapon->stats.weapstats.speed;
                        entitypos->angles.xz += player->target.angles.xz;
                        entitypos->angles.y += player->target.angles.y;
                        AnglesToVectors(entitypos);
                        entitypos->coords.x += (entitypos->vectors.x<<4);
                        entitypos->coords.y += (entitypos->vectors.y<<4);
                        entitypos->coords.z += (entitypos->vectors.z<<4);
                        if(activeweapon->techtype == tt_phaser)
                            slot->entitytype = et_phaser;
                        else slot->entitytype = et_photon_projectile;
                        slot->mobile = true;
                        slot->entitystats.weapon.charge = activeweapon->stats.weapstats.charge;
                        slot->entitystats.weapon.range = activeweapon->stats.weapstats.range;
                    }
                    activeweapon->stats.weapstats.charge = 0;
                }
            }
        }
        
        keys_prior[k_2nd] = key;
    
        kb_ScanGroup(2);
        key = kb_Data[2] & kb_Alpha;
        if(key && (!keys_prior[k_Alpha])){
            if(player->ScreenSelected == SCRN_VIEW_TARG || player->ScreenSelected == SCRN_TACTICAL) {
                char i = (activeweapon->techtype == tt_phaser) ? tt_torpedo : tt_phaser;
                activeweapon = init_SetPointer(&ShipModules, looplimit, i, 0);
            }
        }
        keys_prior[k_Alpha] = key;
        
        kb_ScanGroup(3);
        key = kb_Data[3] & kb_GraphVar;
        if(key && (!keys_prior[k_GraphVar]))
            shields->stats.shieldstats.modulation = randInt(0,255);
        keys_prior[k_GraphVar] = key;
        
        kb_ScanGroup(6);
        key = kb_Data[6] & kb_Enter;
        if( key && !keys_prior[k_Enter] ){
            if(player->ScreenSelected == SCRN_POWER){
                Module_t* module = &ShipModules[moduleselected];
                char health = module->health * 100 / module->maxHealth;
                char power = module->powerReserve * 100 / 256;
                module->online = !module->online;
                if(health < 25 || power < 25) module->online = false;
                
            }
            if(player->ScreenSelected == SCRN_STATUS){
                Module_t* module = &ShipModules[moduleselected];
                char modhealth = module->health * 100 / module->maxHealth;
                if(modulerepairing == moduleselected){
                    modulerepairing = -1;
                    if(modhealth > 25) module->online = true;
                }
                else {
                    modulerepairing = moduleselected;
                    module->online = false;
                    module->powerDraw = module->powerDefault;
                }
            }
        }
        keys_prior[k_Enter] = key;
        
        
        key = kb_Data[6] & kb_Mul;
        if( key && !keys_prior[k_Mul]){
            if(speed < 10 && warpcore->techtype == tt_warpcore) {
                player->position.speed.warp = 10;
                player->position.warpactive = true;
            }
            else {
                player->position.speed.impulse = 0;
                player->position.warpactive = false;
            }
        }
        keys_prior[k_Mul] = key;
        key = kb_Data[6] & kb_Add;
        if( (key && (!keys_prior[k_Add] || player->tick % 5 == 0))){
            if(speed < 10 && speed < topspeed_impulse) player->position.speed.impulse++;
            if(speed >= 10 && speed < topspeed_warp ) player->position.speed.warp++;
        }
        keys_prior[k_Add] = key;
        key = kb_Data[6] & kb_Sub;
        if( (key && (!keys_prior[k_Sub] || player->tick % 5 == 0)) && speed > 0){
            if(speed > 10) player->position.speed.warp--;
            if(speed <= 9) player->position.speed.impulse--;
        }
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
                integrity->health -= damage>>1;
                if(integrity->health < 0) integrity->health = 0;
                damage -= drv;
                if(damage < 0) damage = 0;
            }
            shiphit.internal = damage;
            for(i = 0; i < 2; i++){
                Module_t* module = &ShipModules[randInt(tt_lifesupport-1, tt_warpcore-1)];
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
                    player->position.angles.y++;
                    AnglesToVectors(&player->position);
                    break;
                case SCRN_VIEW_TARG:
                    if(player->target.angles.y < 32) player->target.angles.y+=4;
                    break;
                case SCRN_POWER:
                    if(key && !keys_prior[k_Down]){
                        moduleselected++;
                        module = &ShipModules[moduleselected];
                        if(!module->techtype) moduleselected--;
                    }
                    break;
                case SCRN_STATUS:
                    if(key && !keys_prior[k_Down]){
                        moduleselected++;
                        if(moduleselected > tt_warpcore - 1) moduleselected--;
                        module = &ShipModules[moduleselected];
                        if(!module->techtype) moduleselected = 13;
                    }
                    break;
            }
        }
        keys_prior[k_Down] = key;
        key = kb_Data[7] & kb_Up;
        if( key ){
            Module_t* module;
            switch(player->ScreenSelected){
                case SCRN_VIEW:
                    player->position.angles.y--;
                    AnglesToVectors(&player->position);
                    break;
                case SCRN_VIEW_TARG:
                    if(player->target.angles.y > -32) player->target.angles.y-=4;
                    break;
                case SCRN_POWER:
                    if(key && !keys_prior[k_Up]){
                        moduleselected--;
                        if(moduleselected < 0) moduleselected++;
                    }
                    break;
                case SCRN_STATUS:
                    if(key && !keys_prior[k_Up]){
                        moduleselected--;
                        if(moduleselected < 0) moduleselected++;
                        module = &ShipModules[moduleselected];
                        if(!module->techtype) moduleselected = tt_transporter-1;
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
                    player->position.angles.xz++;
                    AnglesToVectors(&player->position);
                    break;
                case SCRN_VIEW_TARG:
                    if(player->target.angles.xz < 31) player->target.angles.xz+=4;
                    break;
                case SCRN_POWER:
                    if(key && !keys_prior[k_Right]){
                        module = &ShipModules[moduleselected];
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
                    player->position.angles.xz--;
                    AnglesToVectors(&player->position);
                    break;
                case SCRN_VIEW_TARG:
                    if(player->target.angles.xz > -32) player->target.angles.xz-=4;
                    break;
                case SCRN_POWER:
                    if(key && !keys_prior[k_Left]){
                        module = &ShipModules[moduleselected];
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
            PROC_PowerDraw(&ShipModules, modulerepairing);
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
            PROC_PowerCycle(&ShipModules, warpcore, auxpower, modulerepairing, &player, !player->powersource);
        }

        if(player->tick % REPAIR_INTERVAL == 0){
            if(modulerepairing != -1){
                Module_t* repair = &ShipModules[modulerepairing];
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
                if(map_count) GUI_RenderFrame(&sprites, &buffers, &renderbuffer, map_count);
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
                    char anglexz = player->position.angles.xz;
                    char angley = player->position.angles.y;
                    int coords;
                    if(player->ScreenSelected == SCRN_VIEW){
                        char text[] = "[Nav Sensors]";
                        int x = text_GetCenterX(text, vWidth);
                        gfx_PrintStringXY(text, x, yStart+1);
                        
                    }
                    gfx_RenderOrientation(anglexz, angley, xStart+17, yStart+vHeight-25);
                    gfx_Rectangle(xStart + vWidth - 85, yStart+vHeight-23, 85, 23);
                    gfx_PrintStringXY("Sector:", xStart+vWidth-82, yStart+vHeight-20);
                    gfx_SetTextXY(xStart+vWidth-82, yStart+vHeight-10);
                    gfx_PrintUInt(player->position.coords.x>>24, 3);
                    gfx_PrintString(":");
                    gfx_PrintUInt(player->position.coords.y>>24, 3);
                    gfx_PrintString(":");
                    gfx_PrintUInt(player->position.coords.z>>24, 3);
                }
                if(player->ScreenSelected == SCRN_VIEW_TARG){
                    gfx_sprite_t *uncompressed;
                    int view_center_x = vWidth / 2;
                    int view_center_y = vHeight / 2;
                    int weaphealth = activeweapon->health * 100 / activeweapon->maxHealth;
                    int shieldhealth = shields->health * 100 / shields->maxHealth;
                    int sensorhealth = sensors->health * 100 / sensors->maxHealth;
                    char anglexz = player->target.angles.xz;
                    char angley = player->target.angles.y;
                    char text[] = "[Targ Sens]";
                    int x = text_GetCenterX(text, vWidth);
                    gfx_SetTextFGColor(224);
                    gfx_PrintStringXY(text, x, yStart+1);
                    gfx_SetColor(224);
                    gfx_VertLine(xStart + vWidth - 160, yStart + vHeight - 21, 20);
                    gfx_SetTextXY(xStart + vWidth - 150, yStart + vHeight - 20);
                    if(activeweapon->techtype == tt_phaser) gfx_PrintString("Phaser");
                    if(activeweapon->techtype == tt_torpedo) gfx_PrintString("Torpedo");
                    gfx_SetTextXY(xStart+vWidth - 150, yStart + vHeight - 10);
                    if(activeweapon->online){
                        gfx_PrintUInt(weaphealth, 1 + (weaphealth > 9) + (weaphealth > 99));
                        gfx_PrintString("%");
                    } else gfx_PrintString("offline");
                    gfx_SetTextXY(xStart + 75, yStart + vHeight - 20);
                    gfx_PrintString("Shields");
                    gfx_SetTextXY(xStart + 75, yStart + vHeight - 10);
                    if(shields->online){
                        gfx_PrintUInt(shieldhealth, 1 + (shieldhealth > 9) + (shieldhealth > 99));
                        gfx_PrintString("%");
                    } else gfx_PrintString("offline");
                    
                    gfx_SetTextFGColor(255);
                    if(sensorhealth > 25 && (abs(anglexz) <= 32 && abs(angley) <= 32)){
                        char charge = activeweapon->stats.weapstats.charge;
                        char maxCharge = activeweapon->stats.weapstats.maxCharge;
                        int targ_gui_x = view_center_x * anglexz / 33 + view_center_x;
                        int targ_gui_y = view_center_y * angley / 33 + view_center_y;
                        gfx_sprite_t *uncompressed = buffers.uncompressed;
                        gfx_SetColor(224);
                        zx7_Decompress(uncompressed, target_compressed);
                        gfx_TransparentSprite(uncompressed,
                                                xStart + targ_gui_x - 15,
                                                yStart + targ_gui_y - 15);
                       /* gfx_Circle(xStart + targ_gui_x, yStart + targ_gui_y, 15);
                        gfx_Circle(xStart + targ_gui_x, yStart + targ_gui_y, 8); */
                        gfx_HorizLine(xStart + targ_gui_x - 15, yStart + targ_gui_y, 30);
                        gfx_VertLine(xStart + targ_gui_x, yStart + targ_gui_y - 15, 30);
                        gfx_FillCircle(xStart + targ_gui_x, yStart + targ_gui_y, charge * 8 / maxCharge);
                    }
                }
                break;
            case SCRN_TACTICAL:
                GUI_TacticalReport(&ShipModules, looplimit, shields, activeweapon, gfx_initialized[gui_enabled]);
                break;
            case SCRN_STATUS:
                 GUI_StatusReport(&ShipModules, looplimit, moduleselected, modulerepairing);
                break;
            case SCRN_SENSORS:
                GUI_SensorReadout(MapMain, sizeof(MapMain)/sizeof(MapData_t), &player, sensors, gfx_initialized[gui_enabled]);
                break;
            case SCRN_POWER:
                GUI_PowerReport(&ShipModules, looplimit, moduleselected, gfx_initialized[gui_enabled], !player->powersource);
                break;
        }
        gfx_SetClipRegion(0, 0, 320, 240);
        
        
        if(warpcore->techtype == tt_warpcore){
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
        if(gfx_initialized[gui_enabled]){
            if(player->timers[timer_lifesupport]) gfx_DrawLifeSupportAlert(&buffers);
            if(player->timers[timer_corebreach]) gfx_DrawCoreBreachAlert(&buffers);
            if(!(player->tick % 5))gfx_DrawShipStatusIcon(&buffers, integrity, shields, &player);
        }
        gfx_DrawSpeedIndicator(speed, topspeed_warp, topspeed_impulse, gfx_initialized[gui_enabled]);
        integrityhealth = integrity->health * 100 / integrity->maxHealth;
        if(speed){
            bool remainder = player->tick % 2;
            if(!remainder) proc_MoveEntity(&player->position, speed>>1);
            else proc_MoveEntity(&player->position, speed % 2);
        }
        map_MoveObjects(&MapMain, player->tick % 2);
        gfx_BlitBuffer();
        player->tick++;
        player->moduleSelected = moduleselected;
        player->moduleRepairing = modulerepairing;
    }
    while(loopgame);
    gfx_SetDrawScreen();
    boot_ClearVRAM();
    free(buffers.uncompressed);
    free(buffers.rotated);
    free(buffers.scaled);
    gfxinit_FreeAll(&sprites);
    // Cleanup
    ti_CloseAll();
    gfx_SetTextFGColor(0);
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
            gfx_PrintStringXY("trekvfx appvar missing!", 1, 0);
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

char gfx_MainMenu(bool gfx_initialized, buffers_t* buffers){
    bool splash_first_loop = true;
    gfx_sprite_t *uncompressed = buffers->uncompressed;
    char key = 0, i;
    gfx_ZeroScreen();
    if(gfx_initialized){
        zx7_Decompress(uncompressed, splashlogo_compressed);
        gfx_Sprite(uncompressed, 20, 50);
    } else {
        gfx_SetTextFGColor(224);
        gfx_PrintStringXY("[ Warning ]", 5, 120);
        gfx_PrintStringXY("trekicons appvar", 5, 130);
        gfx_PrintStringXY("missing. Game will", 5, 140);
        gfx_PrintStringXY("function but UI", 5, 150);
        gfx_PrintStringXY("elements may be missing", 5, 160);
    }
    gfx_SetTextScale(3,3);
    gfx_SetTextFGColor(26);
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
    gfx_SetTextFGColor(0);
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

