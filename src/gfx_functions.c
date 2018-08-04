
#include <graphx.h>
#include <decompress.h>
#include "equates.h"
#include "gfx/icons.h"
#include "gfx_functions.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"

void lcars_DrawHealthBar(int percent, char scale, int x, int y, bool text){
    gfx_SetColor(0);
    gfx_FillRectangle(x+5, y, 50+(50*(scale==1)), 7);
    gfx_SetColor(255);
    gfx_FillRectangle(x+5, y, percent/scale, 7);
    if(text){
        gfx_SetTextXY(x+55+(50*(scale==1))+7-(2*(scale>1)), y);
        gfx_PrintUInt(percent,1+(percent>9)+(percent>99));
        gfx_PrintString("%  ");
    }
}


void gfx_WipeScreen(){
    gfx_SetColor(0);
    gfx_FillRectangle(23, 21, 274, 164);
    gfx_SetTextXY(xStart, yStart);
}

void PrintHeader(char *text, char x, char y, char xtextOff, char ytextOff){
    gfx_SetColor(247);
    gfx_FillRectangle(x, y, vWidth, 12);
    gfx_SetTextFGColor(0);
    gfx_SetTextBGColor(247);
    gfx_PrintStringXY(text, x+xtextOff, y+ytextOff);
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
}


void DrawGUI(){
    const char *version = "v0.47 alpha";
    char yPos = 230;
    gfx_FillScreen(148);
    gfx_SetColor(74);
    gfx_FillRectangle(20, 19, 280, 170);
    gfx_SetColor(0);
    gfx_FillRectangle(23, 22, 274, 164);
    gfx_SetTextScale(1,1);
    gfx_SetColor(74);
    gfx_SetTextBGColor(74);
    gfx_FillRectangle(10, 227, 300, 13);
    gfx_PrintStringXY("Tactical", 14, yPos);
    gfx_PrintStringXY("Status", 83, yPos);
    gfx_PrintStringXY("Inventory", 140, yPos);
    gfx_PrintStringXY("Power", 226, yPos);
    gfx_PrintStringXY("Map", 280, yPos);
    gfx_SetTextScale(1,1);
    gfx_SetColor(197);
    gfx_FillRectangle(0, 0, 320, 18);
    gfx_SetColor(16);
    gfx_FillRectangle(2, 2, 316, 14);
    gfx_SetTextFGColor(197);
    gfx_SetTextBGColor(16);
    gfx_PrintStringXY("Star Trek Multiplayer ", 32, 5);
    gfx_PrintString(version);
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetColor(255);
    gfx_Line(75, 228, 75, 238);
    gfx_Line(135, 228, 135, 238);
    gfx_Line(218, 228, 218, 238);
    gfx_Line(270, 228, 270, 238);
    gfx_BlitBuffer();
}


void gfx_DrawShipStatusIcon(Module_t* integrity, Module_t* shields){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    char shieldhealth = shields->health * 100 / shields->maxHealth;
    char integhealth = integrity->health * 100 / integrity->maxHealth;
    if(uncompressed = gfx_MallocSprite(shield_alert_width, shield_alert_height)){
        gfx_SetColor(30);
        if(shieldhealth < 50) gfx_SetColor(247);
        if(shieldhealth < 25) gfx_SetColor(224);
        if(shieldhealth == 0 || !shields->online) gfx_SetColor(148);
        gfx_FillRectangle(10, 195, shield_alert_width, shield_alert_height);
        gfx_SetColor(74);
        if(integhealth < 25) gfx_SetColor(224);
        gfx_FillRectangle(16, 204, 16, 12);
        gfx_FillRectangle(32, 209, 10, 3);
        dzx7_Standard(shield_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 10, 195);
        free(uncompressed);
    }
}


void gfx_DrawInventoryStatusIcon(bool status){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(torpedo_alert_width, torpedo_alert_height)){
        gfx_SetColor(36);
        if(status == false) gfx_SetColor(224);
        gfx_FillRectangle(225 - torpedo_alert_width, 203, torpedo_alert_width, torpedo_alert_height);
        dzx7_Standard(torpedo_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 225 - torpedo_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawCoreBreachAlert(bool status){
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(breach_alert_width, breach_alert_height)){
        gfx_SetColor(224);
        if(!status) gfx_SetColor(148);
        gfx_FillRectangle(285 - breach_alert_width, 203, breach_alert_width, breach_alert_height);
        dzx7_Standard(breach_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 285 - breach_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawLifeSupportAlert(bool status){
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(lifesupport_alert_width, lifesupport_alert_height)){
        gfx_SetColor(224);
        if(!status) gfx_SetColor(148);
        gfx_FillRectangle(260 - lifesupport_alert_width, 203, lifesupport_alert_width, lifesupport_alert_height);
        dzx7_Standard(lifesupport_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 260 - lifesupport_alert_width, 203);
        free(uncompressed);
    }
}

void gfx_DrawSpeedIndicator(char speed){
    char i, tierspeed, difference;
    char warpspeeds[10] = {5, 7, 10, 14, 19, 25, 32, 40, 49, 50};
    bool warpspeed = (speed > 4);
    unsigned char yPos = 191, xPos = xStart + 56;
    gfx_SetTextFGColor(0);
    gfx_SetTextBGColor(148);
    // impulse speed bar
    gfx_SetColor(148);
    gfx_FillRectangle(xPos-5, yPos-2, 220, 10);
    gfx_FillRectangle(xPos+1, yPos+9, 88, 12);
    gfx_SetColor(224);
    gfx_FillRectangle(xPos, yPos, 60, 7);
    gfx_SetColor(23);
    gfx_FillRectangle(xPos+60, yPos, 126, 7);
    gfx_SetColor(0);
    for(i=0; i<=3; i++) gfx_Line(i * 15 + xPos, yPos, i * 15 + xPos, yPos+8);
    for(i=0; i<=9; i++) gfx_Line(i * 14 + xPos+60, yPos, i * 14 + xPos+60, yPos+8);
    gfx_Rectangle(xPos, yPos+7, 186, 2);
    gfx_Rectangle(xPos, yPos+8, 90, 14);
    gfx_SetColor(239);
    gfx_SetTextXY(xPos+5, yPos+12);
    if(!warpspeed){
        if(!speed) gfx_PrintString("Full Stop");
        else {
            gfx_PrintUInt(speed,1);
            gfx_PrintString("/4 Impulse");
        }
        gfx_FillRectangle(xPos, yPos, speed * 15, 7);
        gfx_FillRectangle(speed * 15 + xPos - 2, yPos-1, 5, 9);
    } else {
        unsigned char barlen;
        for(i = 0; i < 9; i++)
            if(warpspeeds[i] > speed) break;
        gfx_PrintString("Warp ");
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
    gfx_SetTextBGColor(0);
}
