
#include <graphx.h>
#include <string.h>
#include <tice.h>
#include <compression.h>
#include "equates.h"
#include "gfx_functions.h"
#include "datatypes/mapdata.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"
#include "mymath.h"
#include "gfx/trekgui.h"
#include "gfx/trekvfx.h"
#include <math.h>


const char *trek_version = "v0.72 alpha";


void lcars_DrawHealthBar(int percent, char scale, int x, int y, bool text){
    gfx_SetColor(0);
    gfx_FillRectangle(x+5, y, 50+(50*(scale==1)), 7);
    gfx_SetColor(255);
    gfx_FillRectangle(x+5, y, percent/scale, 7);
    if(text){
        gfx_SetTextXY(x+55+(50*(scale==1))+7-(2*(scale>1)), y);
        gfx_PrintUInt(percent,lcars_GetIntLength(percent));
        gfx_PrintString("% ");
    }
}


void gfx_WipeScreen(){
    gfx_SetColor(0);
    gfx_FillRectangle(xStart, yStart, vWidth, vHeight);
    gfx_SetTextXY(xStart, yStart);
}

void PrintHeader(char *text, char y){
    unsigned int pixellen = text_GetCenterX(text, vWidth);
    gfx_PrintStringXY(text, pixellen, y);
    gfx_SetColor(255);
    gfx_HorizLine(xStart + 1, y + 11, vWidth - 2);
}


void DrawGUI(){
    char yPos = 230;
    gfx_FillScreen(148);
    gfx_SetColor(74);
    gfx_FillRectangle(xStart-3, yStart-3, vWidth+6, vHeight + 6);
    gfx_SetColor(0);
    gfx_FillRectangle(xStart, yStart, vWidth, vHeight);
    gfx_SetTextScale(1,1);
    gfx_SetColor(74);
    gfx_FillRectangle(10, 227, 300, 13);
    gfx_PrintStringXY("Tactical", 14, yPos);
    gfx_PrintStringXY("Status", 83, yPos);
    gfx_PrintStringXY("Sensors", 145, yPos);
    gfx_PrintStringXY("Power", 216, yPos);
    gfx_PrintStringXY("Chat", 270, yPos);
    gfx_SetTextScale(1,1);
    gfx_SetTextFGColor(255);
    gfx_SetColor(255);
    gfx_Line(75, 228, 75, 238);
    gfx_Line(135, 228, 135, 238);
    gfx_Line(208, 228, 208, 238);
    gfx_Line(260, 228, 260, 238);
}


void gfx_DrawShipStatusIcon(Module_t* integrity, Module_t* shields, Player_t *player){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    char shieldhealth = shields->health * 100 / shields->maxHealth;
    char integhealth = integrity->health * 100 / integrity->maxHealth;
    if(uncompressed = gfx_MallocSprite(ship_alert_width, ship_alert_height)){
        gfx_SetColor(30);
        if(shieldhealth < 50) gfx_SetColor(247);
        if(shieldhealth < 25) gfx_SetColor(224);
        if(shieldhealth == 0 || !shields->online) gfx_SetColor(148);
        gfx_FillRectangle(10, 193, ship_alert_width, ship_alert_height);
        if(integhealth < 50) zx7_Decompress(uncompressed, hull_alert_compressed);
        else  zx7_Decompress(uncompressed, hull_normal_compressed);
        gfx_TransparentSprite(uncompressed, 10, 193);
        zx7_Decompress(uncompressed, ship_alert_compressed);
        gfx_TransparentSprite(uncompressed, 10, 193);
        gfx_SetColor(224);
        if(player->damagesection[nacelles-1]) {
            gfx_FillRectangle(45, 193+5, 18, 2);
            gfx_FillRectangle(45, 193+22, 18, 2);
        }
        if(player->damagesection[aft-1]) gfx_FillRectangle(36, 193+13, 10, 3);
        if(player->damagesection[saucer-1]) gfx_FillCircle(20, 193+14, 3);
        free(uncompressed);
    }
}


void gfx_DrawInventoryStatusIcon(bool status){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(torpedo_alert_width, torpedo_alert_height)){
        zx7_Decompress(uncompressed, torpedo_alert_compressed);
        gfx_TransparentSprite(uncompressed, 225 - torpedo_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawCoreBreachAlert(void){
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(breach_alert_width, breach_alert_height)){
        zx7_Decompress(uncompressed, breach_alert_compressed);
        gfx_TransparentSprite(uncompressed, 285 - breach_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawLifeSupportAlert(void){
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(lifesupport_alert_width, lifesupport_alert_height)){
        zx7_Decompress(uncompressed, lifesupport_alert_compressed);
        gfx_TransparentSprite(uncompressed, 260 - lifesupport_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawSpeedIndicator(char speed, char maxspeed_warp, char maxspeed_impulse, bool icons_enabled){
    gfx_sprite_t *uncompressed;
    char i, tierspeed, difference;
    char warpspeeds[11] = {10, 12, 15, 19, 24, 30, 37, 45, 54, 64, 65};
    bool warpspeed = (speed > 9);
    unsigned char yPos = 191, xPos = xStart + 80, limitbarlen = 0;
    gfx_SetTextFGColor(0);
    // impulse speed bar
    gfx_SetColor(148);
    gfx_FillRectangle(xPos-5, yPos-2, 220, 10);
    gfx_SetColor(107);      // change color?
    gfx_FillRectangle(xPos+1, yPos+9, 50, 15);
    gfx_SetColor(224);
    gfx_FillRectangle(xPos, yPos, 60, 7);
    gfx_SetColor(23);
    gfx_FillRectangle(xPos+60, yPos, 140, 7);
    gfx_SetColor(0);
    for(i=0; i<=3; i++) gfx_Line(i * 15 + xPos, yPos, i * 15 + xPos, yPos+8);
    for(i=0; i<=9; i++) gfx_Line(i * 14 + xPos+60, yPos, i * 14 + xPos+60, yPos+8);
    for(i = 0; i < 9; i++)
        if(warpspeeds[i] > maxspeed_warp) break;
    tierspeed = warpspeeds[i-1];
    difference = warpspeeds[i] - tierspeed;
    limitbarlen = (i * 14) + ((maxspeed_warp - tierspeed) * 14 / difference);
    gfx_SetColor(24);
    gfx_FillRectangle(xPos + 60, yPos, limitbarlen, 3);
    gfx_SetColor(0);
    //  gfx_FillRectangle(xPos, yPos, barlen, 7);
    //gfx_FillRectangle(barlen + xPos - 2, yPos-1, 5, 9);
    gfx_Rectangle(xPos, yPos+7, 200, 2);
    gfx_Rectangle(xPos, yPos+8, 52, 17);
    gfx_SetColor(239);
    gfx_SetTextXY(xPos+3, yPos+12);
    if(!warpspeed){
        if(icons_enabled)
            if(uncompressed = gfx_MallocSprite(impulsespeed_icon_width, impulsespeed_icon_height)){
                zx7_Decompress(uncompressed, impulsespeed_icon_compressed);
                gfx_TransparentSprite(uncompressed, xPos+1, yPos+8);
                free(uncompressed);
            }
        gfx_SetTextXY(xPos+27, yPos + 13);
        gfx_PrintUInt(speed, 1);
        gfx_PrintString("/");
        gfx_PrintUInt(maxspeed_impulse, 1);
        gfx_FillRectangle(xPos, yPos, speed * 60 / maxspeed_impulse , 7);
        gfx_FillRectangle(speed * 60 / maxspeed_impulse + xPos - 2, yPos-1, 5, 9);
    } else {
        unsigned char barlen;
        if(icons_enabled)
            if(uncompressed = gfx_MallocSprite(warpspeed_icon_width, warpspeed_icon_height)){
                zx7_Decompress(uncompressed, warpspeed_icon_compressed);
                gfx_TransparentSprite(uncompressed, xPos+1, yPos+8);
                free(uncompressed);
            }
        for(i = 0; i < 9; i++)
            if(warpspeeds[i] > speed) break;
        gfx_SetTextXY(xPos+27, yPos + 13);
        gfx_PrintUInt(i, 1);
        gfx_PrintString(".");
        tierspeed = warpspeeds[i-1];
        difference = warpspeeds[i] - tierspeed;
        gfx_PrintUInt((speed - tierspeed) * 10 / difference, 1);
        barlen = (i * 14) + ((speed - tierspeed) * 14 / difference) + 60;
      //  gfx_FillRectangle(xPos, yPos, barlen, 7);
        gfx_FillRectangle(barlen + xPos - 2, yPos-1, 5, 9);
    }
    gfx_SetTextFGColor(255);
}

void vfx_RenderSparkFlare(animation_t *animate){
    char i, offset_x, offset_y;
    char duration = animate->duration + 1;
    char pixels = randInt(duration, duration + 5);
    gfx_SetColor(31);
    for(i = 0; i < pixels; i++) {
        unsigned int xpos = animate->origin_x + randInt(-2*duration, 2*duration);
        unsigned char ypos = 10 * randInt(duration - 1, duration) + animate->origin_y;
        gfx_SetPixel(xpos, ypos);
    }
    if(++animate->duration == SPARK_ANIM_DURA) animate->duration = 0;
}

void gfx_RenderOrientation(unsigned char anglexz, unsigned char angley, int dialx, unsigned char dialy){
    char vectorx = 10 * byteCos(anglexz - 64) / 127;
    char vectory = 10 * byteSin(anglexz - 64) / 127;
    gfx_SetColor(255);
    gfx_SetTextXY(dialx - 9, dialy - 18);
    gfx_PrintUInt(anglexz,3);
    gfx_Circle(dialx, dialy + 1, 10);
    gfx_Line(dialx, dialy+1, dialx + vectorx, dialy + 1 + vectory);
    gfx_PrintStringXY("Rot", dialx - 9, dialy + 15);
    dialx += 30;
    vectorx = 10 * byteCos(angley) / 127;
    vectory = 10 * byteSin(angley) / 127;
    gfx_SetTextXY(dialx - 9, dialy - 18);
    gfx_PrintUInt(angley,3);
    gfx_Circle(dialx, dialy+1, 10);
    gfx_Line(dialx, dialy+1, dialx + vectorx, dialy+1 + vectory);
    gfx_PrintStringXY("Pit", dialx - 9, dialy + 15);
}


