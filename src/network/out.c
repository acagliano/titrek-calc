#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include <string.h>
#include "../equates.h"
#include "controlcodes.h"
#include "../asm/exposure.h"
#include "network.h"

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


bool ntwk_Login(void) {
    uint24_t text_x = 0;
    uint8_t text_y = 0;

    gfx_ZeroScreen();
    gfx_BlitBuffer();
    text_y = prompt_for("Username:", userinfo.username, 23, text_x, text_y, 0);
    text_y = prompt_for("Password:", userinfo.passwd, 31, text_x, text_y, 1);
    if(!userinfo.username[0] || !userinfo.passwd[0]) return false;

    return ntwk_send(LOGIN,
        PS_STR(userinfo.username),
        PS_STR(userinfo.passwd)
    );
}

bool ntwk_Register(void) {
// input = pointer to preserved username/password data from login function

    if(!gameflags.network) return false;

    gfx_ZeroScreen();
    gfx_SetTextFGColor(255);
    gfx_PrintStringXY("User: ", 0, 0);
    gfx_PrintString(userinfo.username);
    gfx_PrintStringXY("No matching account found!", 0, 10);
    gfx_PrintStringXY("Let's create one...", 0, 20);
    gfx_BlitBuffer();
    prompt_for("Email:", userinfo.email, 63, 0, 30, 0);    // get email address
    if(!userinfo.email[0]) return false;

    return ntwk_send(REGISTER,
        PS_STR(userinfo.username),
        PS_STR(userinfo.passwd),
        PS_STR(userinfo.email)
    );
}

void ntwk_Disconnect(void) {
    ntwk_send_nodata(DISCONNECT);
}
