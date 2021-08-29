#include <graphx.h>
#include <stdint.h>
#include <keypadc.h>
#include <fileioc.h>
#include <tice.h>
#include <hashlib.h>
#include "../gfx/TrekGFX.h"
#include "../classes/ships.h"
#include "../classes/settings.h"
#include "../classes/player.h"
#include "../equates.h"
#include "../flags.h"
#include "../network/network.h"
#include "gui.h"
#include "screens.h"
#include "errors.h"
#include "colors.h"
#include "../asm/exposure.h"
#include "../network/controlcodes.h"
#include "../versioning.h"
#include "text.h"
#include <debug.h>

#define MODICON_START TrekGFX_moduleicons_lifesupport_index
#define TrekGFX_HEADER_SIZE 2
#define MENU_W 100
#define MENU_H 80
gfx_rletsprite_t* modicons[TARG_SENS + 1] = {
    0
};

char options[][50] = {
    "Play Game",
    "Settings",
    "About Game",
    "Exit Game"
};


void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t height, bar_colors_t* colors, bool dynamic_bar_color){
    gfx_SetColor(colors->border);
    gfx_Rectangle(x, y-1, length, height);
    length -= 2;
    gfx_SetColor(colors->bg);
    gfx_FillRectangle(x + 1, y, length, height-2);
    gfx_SetColor(colors->bar);
    if(dynamic_bar_color){
        if(health <= 50) gfx_SetColor(ALL_HEALTHBAR_WARN);
        if(health <= 25) gfx_SetColor(ALL_HEALTHBAR_CRITICAL);
    }
    gfx_FillRectangle(x + 1, y, health * length / 100, height-2);
    gfx_SetColor(255);
}

void stats_RenderPowerIndic(bool warn, uint24_t x, uint8_t y){
    gfx_rletsprite_t* img = (warn) ? icon_power_warn : icon_power_ok;
    gfx_RLETSprite(img, x, y+2);
}

void gfx_InitModuleIcons(void){
    uint24_t src = MODICON_START, ct = TrekGFX_entries_num - MODICON_START, i;
    
    for(i = 0; i < ct; i++){
        modicons[i] = ((gfx_rletsprite_t*)TrekGFX_appvar[src++]);
    }
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

void gfx_RenderMenuOpt(bool active, const char* string, uint24_t x, uint8_t y, uint24_t w, uint8_t h){
    gfx_SetColor(0);
    gfx_Rectangle(x, y, w, h);
    gfx_SetColor(74);
    gfx_SetTextFGColor(255);
    if(active) {
        gfx_SetColor(181);
        gfx_SetTextFGColor(0);
    }
    gfx_FillRectangle(x+1, y+1, w-2, h-2);
    gfx_PrintStringXY(string, x+5, gfx_VCenterText(y, h, 8));
}

void gfx_RenderMenu(char menutext[][50], uint8_t menucount, uint8_t selected, uint24_t x, uint8_t y, uint24_t w, uint8_t h){
    uint8_t i = 0;
    gfx_SetColor(195);
    gfx_Rectangle(x-2, y-2, w+4, h+4);
    gfx_Rectangle(x-1, y-1, w+2, h+2);
    for(i; i<menucount; i++)
        gfx_RenderMenuOpt((selected == i), menutext[i], x, h / menucount * i + y, w, h/menucount);
}

void gfx_RenderMenuTitle(const char* title, uint24_t x, uint8_t y){
    gfx_SetColor(195);
    gfx_SetTextFGColor(0);
    gfx_FillRectangle(x, y, gfx_GetStringWidth(title) + 10, 15);
    gfx_SetTextXY(x+5, y+3);
    gfx_PrintString(title);
}

uint8_t gfx_RenderSplash(gfx_rletsprite_t* splash, uint8_t playgame_err) {
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
            gfx_RenderMenu(options, 4, selected, text_x, text_y, MENU_W, MENU_H);
            if(playgame_err)
                gfx_HighlightedText(err_text[playgame_err], text_x, text_y+MENU_H+10, 255, 192);
            firstrun = false;
        }
        if(gameflags.gfx_error)
            gfx_SplashGFXError(text_x + 140, text_y + 50);
        gfx_SplashNetIndicator(netflags.network_up, 320 - 25, 215);
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

 void gfx_RenderWindow(window_data_t* win){
    uint8_t border_width = win->border_width, y = win->y, h = win->h;
    uint24_t x = win->x, w = win->w;
    gfx_SetColor(win->border_color);
    gfx_FillRectangle(x, y, w, h);
    gfx_SetColor(win->bg_color);
    gfx_FillRectangle(x + border_width, y + border_width, w - (border_width<<1), h - (border_width<<1));
 }

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y, uint8_t flags) {
// returns first newline after prompt
    gfx_SetTextFGColor(228);
    gfx_SetColor(0);
    gfx_FillRectangle(x, y, 320 - (2 * x), 20);
    gfx_PrintStringXY(prompt, x, y);
    gfx_PrintStringXY(">", x, y+10);
    gfx_SetTextXY(x+10, y+10);
    gfx_BlitBuffer();
    user_input(buffer, len, flags);
    return y+22;
}

#define PPT_LEN (LOGIN_TOKEN_SIZE+AES_BLOCKSIZE)
bool gui_Login(uint8_t* key) {
    aes_ctx ctx;
    uint8_t iv[AES_BLOCKSIZE];
    uint8_t ppt[PPT_LEN];
    uint8_t ct[PPT_LEN];
    
    hashlib_AESLoadKey(key, &ctx, 256);         // load secret key
    hashlib_RandomBytes(iv, AES_BLOCKSIZE);     // get IV
    
    // Pad plaintext
    hashlib_AESPadMessage(&settings.login_key, LOGIN_TOKEN_SIZE, ppt, SCHM_DEFAULT);
    
    // Encrypt the login token with AES-256
    hashlib_AESEncrypt(ppt, PPT_LEN, ct, &ctx, iv, AES_MODE_CBC);
    
    return ntwk_send(LOGIN,
        PS_PTR(iv, AES_BLOCKSIZE),
        PS_PTR(ct, PPT_LEN)
    );
    
    // Zero out key schedule, key used, and IV
}

bool gui_NewGame(void) {
    return ntwk_send_nodata(NEW_GAME_REQUEST);
}
