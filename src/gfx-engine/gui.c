#include <graphx.h>
#include <stdint.h>
#include <keypadc.h>
#include "../gfx/TrekGFX.h"
#include "../classes/ships.h"
#include "../classes/settings.h"
#include "../equates.h"
#include "../network/network.h"
#include "gui.h"
#include "../asm/exposure.h"
#include "../network/controlcodes.h"

#define MODICON_START 17
gfx_rletsprite_t* modicons[TARG_SENS + 1] = {
    0
};

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t border, uint8_t bg, uint8_t bar){
    gfx_SetColor(border);
    gfx_Rectangle(x, y-1, length, 10);
    length -= 2;
    gfx_SetColor(bg);
    gfx_FillRectangle(x + 1, y, length, 8);
    gfx_SetColor(bar);
    if(health <= 50) gfx_SetColor(229);
    if(health <= 25) gfx_SetColor(192);
    gfx_FillRectangle(x + 1, y, health * length / 100, 8);
    gfx_SetColor(255);
}

void gfx_InitModuleIcons(void){
    uint24_t src = MODICON_START, ct = TrekGFX_num - MODICON_START, i;
    
    for(i = 0; i < ct; i++){
        modicons[i] = ((gfx_rletsprite_t*)TrekGFX[src++]);
    }
}

uint8_t gfx_RenderSplash(gfx_rletsprite_t *splash) {
    uint24_t text_x = 60;
    uint8_t text_y = 140;
    uint8_t selected = 0;
    gfx_ZeroScreen();
    gfx_RLETSprite(splash, 320 - 260, 20);
    gfx_SetTextFGColor(229);
    gfx_PrintStringXY("Play Game", text_x, text_y);
    gfx_PrintStringXY("Settings", text_x, text_y + 15);
    gfx_PrintStringXY("About Game", text_x, text_y + 30);
    gfx_PrintStringXY("Quit Game", text_x, text_y + 45);
    
    do {
        kb_Scan();
        gfx_SetColor(0);
        gfx_FillCircle(text_x - 10, selected * 15 + text_y + 3, 4);
        if(kb_IsDown(kb_KeyUp)) {
            selected -= (selected > 0);
            wait_kb_release(kb_KeyUp);
        }
        if(kb_IsDown(kb_KeyDown)) {
            selected += (selected < 3);
            wait_kb_release(kb_KeyDown);
        }
        if(kb_IsDown(kb_KeyClear)) {
            selected = 3;
            wait_kb_release(kb_KeyClear);
            break;
        }
        gfx_SetColor(229);
        gfx_FillCircle(text_x - 10, selected * 15 + text_y + 3, 4);

        if(gameflags.network) {
            gfx_SetColor(0);
            gfx_FillRectangle_NoClip(text_x + 100, text_y + 45, gfx_GetStringWidth("Networking disabled!"), 10);
        } else {
            gfx_SetTextFGColor(224);
            gfx_PrintStringXY("Networking disabled!", text_x + 100, text_y + 45);
        }


        gfx_BlitBuffer();
        ntwk_process();
    } while(!kb_IsDown(kb_KeyEnter));
    wait_kb_release(kb_KeyEnter);
    return selected;
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
    gfx_ZeroScreen();
    gfx_BlitBuffer();
    if(!settings.userinfo.username[0])
        text_y = prompt_for("Username:", settings.userinfo.username, 24, text_x, text_y, 0);
    if(!settings.userinfo.passwd[0])
        text_y = prompt_for("Password:", settings.userinfo.passwd, 32, text_x, text_y, 1);
    if(!settings.userinfo.username[0] || !settings.userinfo.passwd[0]) return false;

    return ntwk_send(LOGIN,
        PS_STR(settings.userinfo.username),
        PS_STR(settings.userinfo.passwd)
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
