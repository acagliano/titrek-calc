#include <graphx.h>
#include <compression.h>
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "datatypes/mapdata.h"
#include "gfx_functions.h"
#include "gfx/icons.h"
#include "equates.h"
#include "mymath.h"
#include "gfx/trekicon.h"
#include <math.h>
#include <debug.h>
#include <stdio.h>


void GUI_PowerReport(Module_t *ShipModules, char limit, char selected, bool icons_initialized, bool sourcewarp) {
    char counter = 1, i;
    gfx_sprite_t *battery = NULL;
    gfx_sprite_t *bolt = NULL;
    if(icons_initialized){
        battery = gfx_MallocSprite(battery_icon_width, battery_icon_height);
        bolt = gfx_MallocSprite(power_icon_width, power_icon_height);
        zx7_Decompress(battery, battery_icon_compressed);
        zx7_Decompress(bolt, power_icon_compressed);
    }
    PrintHeader("Power Allocation", yStart+3);
    gfx_PrintStringXY("Reserve", xStart+90, yStart+18);
    gfx_PrintStringXY("Usage", xStart+195, yStart+18);
    
    for(i = 0; i < limit; i++){
        int currpower, powersett;
        int powergen, powermax;
        Module_t *module = &ShipModules[i];
        if(module->techtype){
            if(module->techtype == tt_warpcore || module->techtype == tt_ejectedcore) counter = 14;
            switch(module->techtype){
                case tt_ejectedcore:
                    gfx_SetTextFGColor(229);
                case tt_auxiliary:
                case tt_warpcore:
                    gfx_PrintStringXY(module->techname, xStart+10, yStart+((counter+2)*9) + 5);
                    if(module->techtype == tt_ejectedcore)
                        gfx_PrintStringXY("ejected", xStart+185, yStart+((counter+2)*9) + 5);
                    else if(module->techtype == tt_warpcore){
                        powergen = module->stats.sysstats.powerOutActual;
                        powermax = module->stats.sysstats.powerOut;
                        currpower = module->powerReserve*100/255;
                        if(battery)
                            gfx_TransparentSprite(battery, xStart+80, yStart+((counter+2)*9) + 3);
                        lcars_DrawHealthBar(currpower, 2, xStart+90, yStart+((counter+2)*9) + 5, true);
                        if(bolt)
                            gfx_TransparentSprite(bolt, xStart+182, yStart+((counter+2)*9) + 3);
                        lcars_DrawHealthBar(powergen * 100 / powermax, 2, xStart+190, yStart+((counter+2)*9) + 5, true);
                    }
                    else if(module->techtype == tt_auxiliary){
                        currpower = module->powerReserve * 100 / module->stats.sysstats.auxiliarymax;
                        if(battery)
                            gfx_TransparentSprite(battery, xStart+80, yStart+((counter+2)*9) + 3);
                        lcars_DrawHealthBar(currpower, 1, xStart+90, yStart+((counter+2)*9) + 5, true);
                    }
                    break;
                default:
                    gfx_PrintStringXY(module->techname, xStart+10, yStart+((counter+2)*9));
                    currpower = module->powerReserve*100/255;
                    powersett = module->powerDraw*100/module->powerDefault;
                    lcars_DrawHealthBar(currpower, 2, xStart+80, yStart+((counter+2)*9), true);
                    if(module->online)
                        lcars_DrawHealthBar(powersett, 4, xStart+177, yStart+((counter+2)*9), true);
                    else {
                        gfx_SetTextFGColor(224);
                        gfx_PrintStringXY("offline", xStart+185, yStart+((counter+2)*9));
                    }
                    gfx_Line(xStart+176, yStart+19, xStart+176, yStart+((counter+3)*9));
            }
        }
        counter++;
        gfx_SetTextFGColor(255);
    }
    gfx_SetColor(247);
    
    if(selected < tt_warpcore - 1) gfx_FillRectangle(xStart+5, yStart+(selected+3)*9, 4, 7);
    counter = (sourcewarp) ? 13 : 14;
    gfx_SetColor(38);
    gfx_FillRectangle(xStart+1, yStart+(counter+3)*9 + 5, 8, 7);
    free(battery);
    free(bolt);
}


void GUI_StatusReport(Module_t *ShipModules, char limit, char selected, char repairing) {
    char counter = 1, i;
    PrintHeader("Systems Status", yStart+3);
    
    for(i = 0; i < limit; i++){
        Module_t *module = &ShipModules[i];
        if(module->techtype && module->techtype != tt_auxiliary){
            int health = module->health*100/module->maxHealth;
            gfx_SetColor(36);
            if(!module->online) gfx_SetColor(224);
            if(repairing == i) gfx_SetColor(229);
            gfx_Circle(xStart+18, yStart+3+((counter+1)*9), 1);
            gfx_Circle(xStart+18, yStart+3+((counter+1)*9), 2);
            gfx_PrintStringXY(module->techname, xStart+25, yStart+((counter+1)*9));
            if(module->techtype == tt_ejectedcore) gfx_PrintStringXY("core ejected", xStart+120, yStart+((counter+1)*9));
            else lcars_DrawHealthBar(health, 1, xStart+120, yStart+((counter+1)*9), true);
            gfx_SetColor(255);
            gfx_Line(xStart+228, yStart+17, xStart+228, yStart+((counter+2)*9));
            counter++;
        }
    }
    gfx_SetColor(247);
    if(selected > tt_transporter) selected = tt_transporter;
    if(repairing > tt_transporter) repairing = tt_transporter;
    gfx_FillRectangle(xStart+10, yStart+(selected+2)*9, 4, 7);
    if(repairing != -1) gfx_FillRectangle(xStart+4, (repairing+2)*9+1+yStart, 7, 5);
}

void GUI_TacticalReport(Module_t *ShipModules, char loops, Module_t *shields, Module_t *activeweap, bool icons_initialized){
    // shields GUI
    gfx_sprite_t *uncompressed;
    char i, j = 0;
    unsigned char modulation = shields->stats.shieldstats.modulation;
   // Module_t *shield = &ShipModules[0];         // placeholder, prgm will look for shield
    int power = shields->powerDraw * 100 / shields->powerDefault;
    int resist = shields->stats.shieldstats.resistance;
    gfx_WipeScreen();
    PrintHeader("Tactical Report", yStart+3);
    gfx_SetColor(30);
    gfx_SetTextFGColor(0);
    gfx_FillRectangle(xStart+1, yStart+19, 11, 65);
    gfx_PrintStringXY("S", xStart+3, yStart+ 21);
    gfx_PrintStringXY("H", xStart+3, yStart+ 30);
    gfx_PrintStringXY("I", xStart+3, yStart+ 39);
    gfx_PrintStringXY("E", xStart+3, yStart+ 48);
    gfx_PrintStringXY("L", xStart+3, yStart+ 56);
    gfx_PrintStringXY("D", xStart+3, yStart+ 65);
    gfx_PrintStringXY("S", xStart+3, yStart+ 74);
    gfx_SetTextFGColor(255);
    if(icons_initialized)
        if(uncompressed = gfx_MallocSprite(tactical_width, tactical_height)){
            zx7_Decompress(uncompressed, tactical_compressed);
            gfx_TransparentSprite(uncompressed, xStart + 41, yStart + 30);
            free(uncompressed);
        }
    gfx_SetColor(37);
    if(shields->health * 100 / shields->maxHealth < 25) gfx_SetColor(226);
    if(!shields->online || shields->health == 0) gfx_SetColor(224);
    gfx_Circle(xStart + 50, yStart + 50, 25);
    gfx_Circle(xStart + 50, yStart + 50, 26);
    gfx_Circle(xStart + 50, yStart + 50, 27);
    gfx_PrintStringXY("STATUS: ", xStart + 85, yStart + 34);
    if(shields->online) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
    if(icons_initialized)
        if(uncompressed = gfx_MallocSprite(drv_icon_width, drv_icon_height)){
            zx7_Decompress(uncompressed, drv_icon_compressed);
            gfx_TransparentSprite(uncompressed, xStart + 85, yStart + 43);
            zx7_Decompress(uncompressed, power_icon_compressed);
            gfx_TransparentSprite(uncompressed, xStart + 130, yStart + 44);
            zx7_Decompress(uncompressed, frequency_icon_compressed);
            gfx_TransparentSprite(uncompressed, xStart + 180, yStart + 44);
            free(uncompressed);
        }
    gfx_SetTextXY(xStart + 98, yStart + 45);
    gfx_PrintUInt(resist, 1 + (resist>9) + (resist>99));
    gfx_PrintString(" kJ");
    gfx_SetTextXY(xStart + 143, yStart + 45);
    gfx_PrintUInt(power, lcars_GetIntLength(power));
    gfx_PrintString("%");
    gfx_SetTextXY(xStart + 192, yStart + 45);
    gfx_PrintUInt(modulation, lcars_GetIntLength(modulation));
    gfx_PrintString(" mHz");
    lcars_DrawHealthBar(shields->health * 100 / shields->maxHealth, 1, xStart + 81, yStart + 58, true);
    
    gfx_SetColor(224);
    gfx_FillRectangle(xStart+1, yStart+90, 11, 75);
    gfx_PrintStringXY("W", xStart+3, yStart+ 96);
    gfx_PrintStringXY("E", xStart+3, yStart+ 105);
    gfx_PrintStringXY("A", xStart+3, yStart+ 114);
    gfx_PrintStringXY("P", xStart+3, yStart+ 123);
    gfx_PrintStringXY("O", xStart+3, yStart+ 132);
    gfx_PrintStringXY("N", xStart+3, yStart+ 141);
    gfx_PrintStringXY("S", xStart+3, yStart+ 150);
    for(i = tt_phaser - 1; i < loops; i++){
        Module_t *weap = &ShipModules[i];
        bool armed = false;
        if(weap->techtype == tt_phaser || weap->techtype == tt_torpedo){
            char damage_shield = weap->stats.weapstats.damage_shield,
            damage_hull = weap->stats.weapstats.damage_hull;
            char health = weap->health * 100 / weap->maxHealth;
            char power = weap->powerDraw * 100 / weap->powerDefault;
            int rect_start_x = xStart + 16;
            int rect_start_y = j * 30 + yStart + 100;
            gfx_SetColor(224);
            if(weap == activeweap){
                gfx_SetColor(37);
                gfx_FillRectangle(rect_start_x - 2, rect_start_y, 140, 26);
            }
            gfx_SetColor(214);
            gfx_SetTextFGColor(0);
            gfx_FillRectangle(rect_start_x, rect_start_y+2, 135, 22);
            gfx_SetTextXY(rect_start_x + 6, rect_start_y + 4);
            gfx_PrintString(weap->stats.weapstats.weapname);
            gfx_PrintString(" ");
            if(weap->techtype == tt_phaser) gfx_PrintString("Phaser");
            else if(weap->techtype == tt_torpedo) gfx_PrintString("Torpedo");
           
            gfx_SetTextFGColor(0);
            if(icons_initialized)
                if(uncompressed = gfx_MallocSprite(hull_dmg_icon_width, hull_dmg_icon_height)){
                    zx7_Decompress(uncompressed, shield_dmg_icon_compressed);
                    gfx_TransparentSprite(uncompressed, rect_start_x+2, rect_start_y+12);
                    zx7_Decompress(uncompressed, hull_dmg_icon_compressed);
                    gfx_TransparentSprite(uncompressed, rect_start_x + 31, rect_start_y + 12);
                    zx7_Decompress(uncompressed, health_icon_compressed);
                    gfx_TransparentSprite(uncompressed, rect_start_x + 60, rect_start_y + 12);
                    zx7_Decompress(uncompressed, power_icon_compressed);
                    gfx_TransparentSprite(uncompressed, rect_start_x + 97, rect_start_y + 12);
                    free(uncompressed);
                }
            gfx_SetTextXY(rect_start_x + 13, rect_start_y + 14);
            gfx_PrintUInt(damage_shield, lcars_GetIntLength(damage_shield));
            
            gfx_SetTextXY(rect_start_x + 42, rect_start_y + 14);
            gfx_PrintUInt(damage_hull, lcars_GetIntLength(damage_hull));
           
            gfx_SetTextXY(rect_start_x + 71, rect_start_y + 14);
            gfx_PrintUInt(health, lcars_GetIntLength(health));
           
            gfx_SetTextXY(rect_start_x + 107, rect_start_y + 14);
            gfx_PrintUInt(power, lcars_GetIntLength(power));
            j++;
        }
    }
}

void GUI_SensorReadout(MapData_t *map, unsigned int map_size, Player_t *player, Module_t *sensors, bool icons){
    int i;
    double val = 180/M_PI;
    gfx_sprite_t *uncompressed;
    unsigned int coord_temp;
    unsigned int sens_range = sensors->stats.sysstats.sensor_range;
    int sens_health = sensors->health * 100 / sensors->maxHealth;
    int sens_power = sensors->powerDraw * 100 / sensors->powerDefault;
    unsigned int sens_scrn_x_start = xStart + 148;
    unsigned int sens_scrn_y_start = yStart + 16;
    unsigned int sens_scrn_origin_x = (vWidth + xStart - sens_scrn_x_start) / 2;
    int xmid = sens_scrn_x_start + sens_scrn_origin_x;
    int ymid = sens_scrn_y_start + sens_scrn_origin_x;
    unsigned char player_angle_xz = player->position.angles.xz;
    unsigned char player_angle_y = player->position.angles.y;
    char line_x2 = 74 * byteSin(player->sensor_gui_angle) / 127;
    char line_y2 = 74 * byteCos(player->sensor_gui_angle) / 127;
    PrintHeader("Sensor Readout", yStart+3);
    gfx_SetColor(107);
    gfx_FillRectangle(sens_scrn_x_start, sens_scrn_y_start, sens_scrn_origin_x<<1, sens_scrn_origin_x<<1);
    gfx_SetColor(0);
    gfx_FillCircle(xmid, ymid, sens_scrn_origin_x);
    gfx_RenderOrientation(player_angle_xz, player_angle_y, xStart + 17, yStart + vHeight - 25);
    sens_range = sens_range * sens_power / 100;
    if(sens_health < 50) sens_range = sens_range * sens_health / 50;
    gfx_SetColor(255);
    gfx_VertLine(xmid, sens_scrn_y_start, vHeight - 18);
    gfx_HorizLine(xmid - sens_scrn_origin_x, ymid, sens_scrn_origin_x * 2);
    gfx_PrintStringXY("+x", xmid - 15, yStart + 20);
    gfx_PrintStringXY("-z", xmid + sens_scrn_origin_x - 20, ymid - 10);
    gfx_PrintStringXY("-x", xmid - 15, yStart + vHeight - 15);
    gfx_PrintStringXY("+z", xmid - sens_scrn_origin_x + 5, ymid - 10);
    gfx_PrintStringXY("Coordinates:", xStart + 3, yStart + 16);
    gfx_PrintStringXY("sect", xStart + 9, yStart + 26);
    gfx_PrintStringXY("coord", xStart + 51, yStart + 26);
    gfx_SetTextXY(xStart + 2, yStart + 36);
    gfx_PrintString("X: ");
    coord_temp = player->position.coords.x>>8;
    gfx_PrintUInt(coord_temp>>16, 3);
    gfx_PrintString("  |  ");
    gfx_PrintUInt(coord_temp&0xffff, 5);
    gfx_SetTextXY(xStart + 2, yStart + 46);
    gfx_PrintString("Y: ");
    coord_temp = player->position.coords.y>>8;
    gfx_PrintUInt(coord_temp>>16, 3);
    gfx_PrintString("  |  ");
    gfx_PrintUInt(coord_temp&0xffff, 5);
    gfx_SetTextXY(xStart + 2, yStart + 56);
    gfx_PrintString("Z: ");
    coord_temp = player->position.coords.z>>8;
    gfx_PrintUInt(coord_temp>>16, 3);
    gfx_PrintString("  |  ");
    gfx_PrintUInt(coord_temp&0xffff, 5);
    if(icons && (uncompressed = gfx_MallocSprite(range_icon_width, range_icon_height))){
        zx7_Decompress(uncompressed, range_icon_compressed);
        gfx_TransparentSprite(uncompressed, xStart + 2, yStart + 66);
        free(uncompressed);
    }
    if(sensors->online){
        gfx_SetTextXY(xStart + 20, yStart + 68);
        gfx_PrintUInt(sens_range, lcars_GetIntLength(sens_range));
        gfx_PrintString(" / ");
        gfx_PrintUInt(sensors->stats.sysstats.sensor_range, lcars_GetIntLength(sensors->stats.sysstats.sensor_range));
    }
    else gfx_PrintStringXY("offline", xStart + 20, yStart + 68);
    if(!sensors->online) return;
    gfx_SetColor(191);
    gfx_Line(xmid, ymid, line_x2 + xmid, line_y2 + ymid);
    if(icons && (uncompressed = gfx_MallocSprite(ship_icon_width, ship_icon_height))){
        gfx_sprite_t *rotated;
        zx7_Decompress(uncompressed, ship_icon_compressed);
        if(rotated = gfx_MallocSprite(ship_icon_width, ship_icon_height)){
            gfx_RotateSprite(uncompressed, rotated, player_angle_xz);
            gfx_TransparentSprite(rotated, xmid - (ship_icon_width>>1), ymid - (ship_icon_height>>1));
            free(rotated);
        } else
            gfx_TransparentSprite(uncompressed, xmid - (ship_icon_width>>1), ymid - (ship_icon_height>>1));
        free(uncompressed);
    }
    sens_range *= sens_range;
    for(i = 0; i < map_size; i++){
        MapData_t *entity = &map[i];
        if(entity->entitytype){
            unsigned long distance;
            int dx = (entity->position.coords.x - player->position.coords.x)>>8;
            int dy = (entity->position.coords.y - player->position.coords.y)>>8;
            int dz = (entity->position.coords.z - player->position.coords.z)>>8;
            distance = r_GetDistance(dx, dy, dz);
            if(distance <= sens_range){
                int render_x, render_y, conv_dist;
                signed int tempangle = 255 * atan2(dz, dx) * val / 360;
                unsigned char anglexz = (tempangle < 0) ? 255 + tempangle : tempangle, angley;
                tempangle = 255 * atan2(dy, dx) * val / 360;
                angley = (tempangle < 0) ? 255 + tempangle : tempangle;
                conv_dist = distance * sens_scrn_origin_x / sens_range;
                render_x = conv_dist * byteCos(anglexz - 64) / 127;
                conv_dist = distance * sens_scrn_origin_x / sens_range;
                render_y = conv_dist * byteSin(anglexz - 64) / 127;
                gfx_SetColor(242);
                gfx_FillCircle(xmid + render_x, ymid + render_y, 3);
            }
        }
    }
    player->sensor_gui_angle += 2;
}
