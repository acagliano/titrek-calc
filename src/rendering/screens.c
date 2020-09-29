
#include <tice.h>
#include <string.h>
#include <graphx.h>
#include <compression.h>
#include <debug.h>
#include "../classes/tech.h"
#include "../classes/ships.h"
#include "../classes/screens.h"
#include "../classes/modules.h"

#include "../gfx/moduleicons.h"
#include "../gfx/TrekGFX.h"
#include "../rendering/gui.h"

#include "../equates.h"
#include "screens.h"

#define gfx_RectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_Rectangle((x1), (y1), (w), (h));
#define gfx_FillRectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_FillRectangle((x1), (y1), (w), (h));

char mains_title[] = "[MAIN]";
char tact_title[] = "[TACT]";

char moduledb[][12] = {
    "LifeSup",
    "Core",
    "WarpDr",
    "Impulse",
    "NavSens",
    "Transpt",
    "Shields",
    "Armor",
    "Phasers",
    "Torpedo",
    "TargSens"
};
char modulenames[][20] = {
    "Life Support",
    "Warp Core",
    "Warp Drive",
    "Sublight Engines",
    "Nav Sensors",
    "Transporter",
    "Shields",
    "Hull Armor",
    "Phasers",
    "Torpedo",
    "Targetting Sensors"
};

int text_GetCenterX(char* string){
    return (LCD_WIDTH - gfx_GetStringWidth(string)) / 2;
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100);
}



void Screen_RenderUI(void){
    Screen_Background(screen);
    gfx_SetTextFGColor(255);
    switch(screen & 0xff){
        case SCRN_OFF:
            break;
        case SCRN_SENS:
            break;
        case SCRN_TACT:
            Screen_UITacticalStats(Ship.hull, Ship.system, MAX_MODULES, select.tactical);
            if(screen > 0xff)
                Overlay_UIModuleInfo();
            break;
        case SCRN_MAINS:
            Screen_UISystemStats(Ship.system, MAX_MODULES, select.mains);
            if(screen > 0xff)
                Overlay_UIModuleInfo();
            break;
        case SCRN_TRANSPORT:
            break;
        case SCRN_CARGO:
            break;
    }
    gfx_SetTextFGColor(0);
    return;
}

void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected){
    unsigned char i, j = 0, cur_y;
    int cur_x;
    gfx_RLETSprite(mains_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(mains_title, viewer_x + 26, viewer_y - 1);
    for(i = 0; i < syscount; i++){
        // loop module health display
        module_t* module = &systems[i];
        if(module->techclass == mSystem){
            cur_y = j * 16 + viewer_y + 14;
            cur_x = viewer_x + 40;
            if(selected == i) {
                gfx_SetColor(230);
                gfx_FillRectangle(cur_x - 6, cur_y, 6, 16);
            }
            gfx_SetColor(255);
            gfx_SetTextXY(cur_x, cur_y);
            module_RenderGeneral(module, cur_x, cur_y);
            j++;
        }
    }
    return;
}

void Screen_UITacticalStats(hull_t hull, module_t* systems, uint24_t syscount, uint24_t selected){
    unsigned char i, j=0, cur_y = viewer_y + 30;
    int cur_x = viewer_x;
    uint24_t shield_health = 0, shield_resist = 0, shield_num = 0;
    bool shields_active = false;
    gfx_RLETSprite(tactical_display, viewer_x, viewer_y - 3);
    gfx_PrintStringXY(tact_title, viewer_x + 26, viewer_y - 1);
    for(i = 0; i < syscount; i++){
        int temp_y = 16 * j + viewer_y + 14;
        int temp_x = cur_x + 90;
        module_t* module = &systems[i];
        if(module->techclass == mTactical){
            if(selected == i) {
                gfx_SetColor(230);
                gfx_FillRectangle(temp_x - 6, temp_y, 6, 16);
            }
            module_RenderGeneral(module, temp_x, temp_y);
            if(module->techtype == SHIELD){
                if(module_OnlineState(module)){
                    shields_active = true;
                    shield_health += module->health;
                    shield_num++;
                }
            }
            j++;
        }
    }
    shield_health /= shield_num;
    gfx_DrawShieldDisp(shields_active, shield_health, top, cur_x, cur_y - 1);
    gfx_DrawShieldDisp(shields_active, shield_health, side, cur_x + 40, cur_y - 1);
    cur_y += 42;
    gfx_RLETSprite(icon_hullinteg, cur_x, cur_y - 1);
    cur_x += 12;
    stats_DrawHealthBar(hull.health, 60, cur_x, cur_y, 10, 33, 107, 74);
}





void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y){
    int24_t health = module->health;
    uint8_t bar = (module->techtype < SYS_MAX) ? 77 : 29;
    uint24_t x_space = vWidth - (x + 10);
    uint24_t width = (x_space > 200) ? 200 : x_space;
    gfx_RectangleColor(229, x, y, width, 16);
    if(module->techclass){
        uint8_t techtype = module->techtype;
        uint24_t barwidth;
        x_space -= 90;
        barwidth = (x_space > 100) ? 100 : x_space;
        if(module_OnlineState(module)){
            gfx_FillRectangleColor(3, x + 1, y + 1, width - 2, 14);}
        else{
            gfx_FillRectangleColor(96, x + 1, y + 1, width - 2, 14);}
        gfx_FillRectangleColor(0, x+1, y+1, 14, 14);
        gfx_RLETSprite(modicons[techtype], x + 1, y + 1);
        gfx_PrintStringXY(moduledb[techtype], x + 20, y + 5);
        stats_DrawHealthBar(health, barwidth, x + 80, y + 4, 10, 0, 149, bar);
    }
    else {
        gfx_FillRectangleColor(74, x + 1, y + 1, width - 2, 14);
        gfx_PrintStringXY("unassigned", x + 10, y + 5);
    }
}


void Overlay_UIModuleInfo(void) {
    uint24_t x = 60, width = 200;
    uint8_t y = 30, height = 110;
    uint24_t text_x = x + 4;
    uint8_t text_y = y + 4;
    gfx_SetTextFGColor(0);
    gfx_RenderWindow(x, y, width, height, 139, 205, 3);
    gfx_FillRectangleColor(139, x + 2, y + 2, width - 4, 16);
    if(!ModuleInfo.modclass) {
        gfx_PrintStringXY("No Module Info Loaded", text_x + 20, text_y + 2);
        return;
    }
    uint8_t state = ModuleInfo.state;
    uint8_t techtype = ModuleInfo.modtype;
    uint8_t health = health_GetPercent(&ModuleInfo.health);
    uint8_t power = power_GetReservePercent(&ModuleInfo.power);
    
    gfx_RLETSprite(modicons[techtype], text_x + 2, y + 2);
    gfx_PrintStringXY(modulenames[techtype], text_x + 20, text_y + 2);
    text_y += 17;
    gfx_RLETSprite(icon_health, text_x, text_y - 2);
    stats_DrawHealthBar(health, 130, text_x + 15, text_y, 10, 0, 149, 77);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(health, 1 + (health > 9) + (health > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_RLETSprite(icon_sourcereserve, text_x, text_y - 2);
    stats_DrawHealthBar(power, 130, text_x + 15, text_y, 10, 0, 149, 77);
    gfx_SetTextXY(text_x + 15 + 135, text_y);
    gfx_PrintUInt(power, 1 + (power > 9) + (power > 99));
    gfx_PrintString("%");
    text_y += 12;
    gfx_PrintStringXY("Status: ", text_x + 2, text_y);
    if(state) gfx_PrintString("Online");
    else gfx_PrintString("Offline");
}

void Screen_Background(uint8_t active) {
    int i;
    gfx_SetTextFGColor(255);
    gfx_ZeroScreen();
    gfx_RLETSprite(shipinterior_left, 0, 0);
    gfx_RLETSprite(shipinterior_right, 160, 0);
    for(i = 0; i < 320; i++){
        gfx_SetColor(0);
        gfx_Rectangle(i * 64, 216, 64, 20);
    }
    gfx_SetColor(116);
    if(active) gfx_FillRectangle(--active*64+1, 217, 62, 18);
    gfx_PrintStringXY("Sensors", 4, lcars_texty);
    gfx_PrintStringXY("Tactical", 68, lcars_texty);
    gfx_PrintStringXY("MainSys", 134, lcars_texty);
    gfx_PrintStringXY("Repair", 200, lcars_texty);
    gfx_PrintStringXY("Cargo", 268, lcars_texty);
    gfx_SetTextFGColor(148);
    gfx_PrintStringXY(versionstr, 160 - (7 * strlen(versionstr) / 2), 1);
    gfx_SetTextFGColor(0);
}

char *strify_version(char *str, uint8_t *version){
  char buf[12];
  int len,inx,vi;
  len=inx=0;
  for (vi=0; vi<3; vi++){
    sprintf(&buf,"%d\.",version[vi]);
    len=strlen(&buf);
    memcpy(str+inx,&buf,len);
    inx+=len;
  }
  str[inx-1]=0;
  return str;
}
