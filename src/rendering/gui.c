#include <graphx.h>
#include <stdint.h>
#include <keypadc.h>
#include <fileioc.h>
#include <tice.h>
#include "../gfx/TrekGFX.h"
#include "../classes/ships.h"
#include "../classes/settings.h"
#include "../classes/player.h"
#include "../equates.h"
#include "../network/network.h"
#include "gui.h"
#include "screens.h"
#include "../asm/exposure.h"
#include "../network/controlcodes.h"
#include <debug.h>

#define MODICON_START 15
#define TrekGFX_HEADER_SIZE 2
#define MENU_W 100
#define MENU_H 80
gfx_rletsprite_t* modicons[TARG_SENS + 1] = {
    0
};

char options[][12] = {
    "Play Game",
    "Settings",
    "About Game",
    "Exit Game"
};

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t height, uint8_t border, uint8_t bg, uint8_t bar){
    gfx_SetColor(border);
    gfx_Rectangle(x, y-1, length, height);
    length -= 2;
    gfx_SetColor(bg);
    gfx_FillRectangle(x + 1, y, length, height-2);
    gfx_SetColor(bar);
    if(health <= 50) gfx_SetColor(229);
    if(health <= 25) gfx_SetColor(192);
    gfx_FillRectangle(x + 1, y, health * length / 100, height-2);
    gfx_SetColor(255);
}

void gfx_InitModuleIcons(void){
    uint24_t src = MODICON_START, ct = TrekGFX_entries_num - MODICON_START, i;
    
    for(i = 0; i < ct; i++){
        modicons[i] = ((gfx_rletsprite_t*)TrekGFX_appvar[src++]);
    }
}

void gfx_GetVersion(void){
    ti_var_t appv;
    if(appv = ti_Open("TrekGFX", "r")){
        ti_Read(&gfx_version, 2, 1, appv);
        ti_Close(appv);
    }
}

uint8_t gfx_getCustomPackCount(void){
    ti_var_t appvar;
    unsigned int *table;
    void *base;
    if(!(appvar = ti_Open("TrekGFX", "r"))) return 0;
    table = base = (char*)ti_GetDataPtr(appvar) + TrekGFX_HEADER_SIZE;
    return *table;
}

void gfx_VersionCheck(void){
    if((gfx_version[0] == 0xff) && (gfx_version[1] == 0xff)) {
        gameflags.gfx_custom = true;
        if(gfx_getCustomPackCount() == TrekGFX_entries_num) return;
        gameflags.gfx_error = true;
        return;
    }
    if((gfx_version[0] == gfx_reqd[0]) && (gfx_version[1] == gfx_reqd[1]))
        return;
    gameflags.gfx_error = true;
}

void gfx_SplashGFXError(uint24_t x, uint8_t y){
    gfx_SetTextFGColor(195);
    gfx_SetTextXY(x, y);
    gfx_PrintString("= GFX ERROR =");
    gfx_SetTextXY(x, y+10);
    if(gameflags.gfx_custom)
        gfx_PrintString("Custom GFX");
    else {
        gfx_PrintString("Installed: ");
        gfx_SetTextXY(x + 70, y+10);
        gfx_PrintUInt(gfx_version[0], num_GetLength(gfx_version[0]));
        gfx_PrintString(".");
        gfx_PrintUInt(gfx_version[1], num_GetLength(gfx_version[1]));
    }
    gfx_SetTextXY(x, y+20);
    
    if(gameflags.gfx_custom)
        gfx_PrintString("Err: Bad Count");
    else {
        gfx_PrintString("Required: ");
        gfx_SetTextXY(x + 70, y+20);
        gfx_PrintUInt(gfx_reqd[0], num_GetLength(gfx_reqd[0]));
        gfx_PrintString(".");
        gfx_PrintUInt(gfx_reqd[1], num_GetLength(gfx_reqd[1]));
    }
}

void gfx_SplashNetIndicator(bool status, uint24_t x, uint8_t y){
    gfx_SetTextFGColor(229);
    if(status)
        gfx_RLETSprite(icon_netup, x, y);
    else
        gfx_RLETSprite(icon_netdown, x, y);
    gfx_PrintStringXY("NET", x - 30, y + 5);
}

void gfx_RenderMenuOpt(bool active, const char* string, uint24_t x, uint8_t y){
    gfx_SetColor(0);
    gfx_Rectangle(x, y, MENU_W, MENU_H/4);
    gfx_SetColor(74);
    gfx_SetTextFGColor(255);
    if(active) {
        gfx_SetColor(181);
        gfx_SetTextFGColor(0);
    }
    gfx_FillRectangle(x+1, y+1, MENU_W-2, (MENU_H/4)-2);
    gfx_PrintStringXY(string, x+10, y+7);
}

void gfx_RenderMenu(uint8_t selected, uint24_t x, uint8_t y){
    uint8_t i = 0;
    gfx_SetColor(195);
    gfx_Rectangle(x-2, y-2, MENU_W+4, MENU_H+4);
    gfx_Rectangle(x-1, y-1, MENU_W+2, MENU_H+2);
    for(i; i<4; i++)
        gfx_RenderMenuOpt((selected == i), options[i], x, MENU_H / 4 * i + y);
}

void gfx_RenderVersion(uint24_t x, uint8_t y){
    gfx_SetColor(195);
    gfx_SetTextFGColor(0);
    gfx_FillRectangle(x, y, 65, 15);
    gfx_SetTextXY(x+5, y+3);
    gfx_PrintString("v ");
    gfx_PrintString(versionstr);
}

uint8_t gfx_RenderSplash(gfx_rletsprite_t* splash) {
    bool firstrun = true;
    uint24_t text_x = 60;
    uint8_t text_y = 130;
    uint8_t selected = 0;
    sk_key_t key;
    gfx_ZeroScreen();
    gfx_SetTextFGColor(0);
    gfx_RLETSprite(splash, 320 - 260, 20);
    do {
        key = getKey();
        if(key == sk_Up) {
            selected -= (selected > 0);
            wait_kb_release(kb_KeyUp);
        }
        if(key == sk_Down) {
            selected += (selected < 3);
            wait_kb_release(kb_KeyDown);
        }
        if(key == sk_Clear) {
            selected = 3;
            wait_kb_release(kb_KeyClear);
            break;
        }
        if(key || firstrun){
            gfx_RenderVersion(text_x - 2, text_y-15);
            gfx_RenderMenu(selected, text_x, text_y);
            firstrun = false;
        }
        if(gameflags.gfx_error)
            gfx_SplashGFXError(text_x + 140, text_y + 50);
        gfx_SplashNetIndicator(gameflags.network, 320 - 25, 215);
        gfx_BlitBuffer();
        ntwk_process();
    } while(key != sk_Enter);
    wait_kb_release(kb_KeyEnter);
    return selected;
}

void gfx_DrawShieldDisp(bool active, uint8_t health, gfx_rletsprite_t* img, uint24_t x, uint8_t y){
    uint8_t window_r = 18;
    uint24_t img_x = x + window_r - (img->width>>1);
    uint8_t img_y = y + window_r - (img->height>>1);
    if(active){
        uint24_t circle_x = x + window_r;
        uint8_t circle_y = y + window_r, i;
        uint8_t bubble_width = (health > 75) ? 4 :
                                (health > 50) ? 3 :
                                (health > 25) ? 2 :
                                (health > 0) ? 1 : 0;
        gfx_SetColor(29);
        for(i = 0; i < bubble_width; i++)
            gfx_Circle(circle_x, circle_y, 14 + i);
    }
    gfx_RLETSprite(img, img_x, img_y);
}

 void gfx_RenderWindow(uint24_t x, uint8_t y, uint24_t width, uint8_t height, uint8_t borderColor, uint8_t windowColor, uint8_t borderSize){
    gfx_SetColor(borderColor);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(windowColor);
    gfx_FillRectangle(x + borderSize, y + borderSize, width - (borderSize<<1), height - (borderSize<<1));
 }

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y, uint8_t flags) {
// returns first newline after prompt
    gfx_SetTextFGColor(228);
    gfx_PrintStringXY(prompt, x, y);
    gfx_PrintStringXY(">", x, y+10);
    gfx_SetTextXY(x+10, y+10);
    gfx_BlitBuffer();
    user_input(buffer, len, flags);
    return y+22;
}

bool gui_Login(void) {
    uint24_t text_x = 0;
    uint8_t text_y = 0;
    if(getKey() == sk_Del) memset(&settings.userinfo, 0, sizeof(userinfo_t));
    gfx_ZeroScreen();
    gfx_BlitBuffer();
    if(!settings.userinfo.username[0])
        text_y = prompt_for("Username:", settings.userinfo.username, 24, text_x, text_y, 0);
    if(!settings.userinfo.passwd[0])
        text_y = prompt_for("Password:", settings.userinfo.passwd, 32, text_x, text_y, 1);
    if(!settings.userinfo.username[0] || !settings.userinfo.passwd[0]){
        // if one or both fields left blank, clear the buffers
        memset(&settings.userinfo, 0, sizeof(userinfo_t));
        return false;
    }

    return ntwk_send(LOGIN,
        PS_STR(settings.userinfo.username),
        PS_STR(settings.userinfo.passwd),
        PS_ARR(version),
        PS_ARR(gfx_version)
    );
}

bool gui_Register(void) {
// input = pointer to preserved username/password data from login function
    char email[64];
    if(!gameflags.network) return false;

    gfx_ZeroScreen();
    gfx_SetTextFGColor(255);
    gfx_PrintStringXY("User: ", 0, 0);
    gfx_PrintString(settings.userinfo.username);
    gfx_PrintStringXY("No matching account found!", 0, 10);
    gfx_PrintStringXY("Let's create one...", 0, 20);
    gfx_BlitBuffer();
    prompt_for("Email:", &email, 63, 0, 30, 0);    // get email address
    if(!email[0]) return false;

    return ntwk_send(REGISTER,
        PS_STR(settings.userinfo.username),
        PS_STR(settings.userinfo.passwd),
        PS_STR(email)
    );
}

bool gui_NewGame(void) {
    return ntwk_send_nodata(NEW_GAME_REQUEST);
}
