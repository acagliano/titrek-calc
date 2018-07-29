
#include <graphx.h>
#include <decompress.h>
#include "equates.h"
#include "gfx/icons.h"
#include "gfx_functions.h"
#include "datatypes/shipmodules.h"

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
    const char *version = "v0.0.41 alpha";
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
    gfx_PrintStringXY("Star Trek Multiplayer ", 28, 5);
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
    if(uncompressed = gfx_MallocSprite(shield_alert_width, shield_alert_height)){
        gfx_SetColor(36);
        if(shields->health * 100 / shields->maxHealth < 50) gfx_SetColor(197);
        if(shields->health * 100 / shields->maxHealth < 25) gfx_SetColor(224);
        if(shields->health * 100 / shields->maxHealth == 0 || !shields->online) gfx_SetColor(148);
        gfx_FillRectangle(10, 195, shield_alert_width, shield_alert_height);
        gfx_SetColor(74);
        if(integrity->health * 100 / integrity->maxHealth < 25) gfx_SetColor(224);
        gfx_FillRectangle(16, 204, 16, 12);
        gfx_FillRectangle(32, 209, 10, 3);
        dzx7_Standard(shield_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 10, 195);
        free(uncompressed);
    }
}

void gfx_DrawPowerStatusIcon(bool status){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(power_alert_width, power_alert_height)){
        gfx_SetColor(36);
        if(status == false) gfx_SetColor(224);
        gfx_FillRectangle(310 - power_alert_width, 203, power_alert_width, power_alert_height);
        dzx7_Standard(power_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 310 - power_alert_width, 203);
        free(uncompressed);
        gfx_BlitBuffer();
    }
}

void gfx_DrawInventoryStatusIcon(bool status){
    // status will be false for compromised, or true for good
    gfx_sprite_t *uncompressed;
    if(uncompressed = gfx_MallocSprite(torpedo_alert_width, torpedo_alert_height)){
        gfx_SetColor(36);
        if(status == false) gfx_SetColor(224);
        gfx_FillRectangle(290 - torpedo_alert_width, 203, torpedo_alert_width, torpedo_alert_height);
        dzx7_Standard(torpedo_alert_compressed, uncompressed);
        gfx_TransparentSprite(uncompressed, 290 - torpedo_alert_width, 203);
        free(uncompressed);
    }
}
