#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <srldrvce.h>
#include <graphx.h>
#include <string.h>
#include "../equates.h"
#include "usb.h"
#include "controlcodes.h"
#include "../asm/exposure.h"

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y, uint8_t flags){
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
    uint8_t ctlcode = LOGIN;
    uint24_t text_x = 0;
    uint8_t text_y = 0;
    uint24_t packet_len;
    uint8_t un_len, pass_len;

    if(!gameflags.network) return false;

    gfx_ZeroScreen();
    gfx_BlitBuffer();
    text_y = prompt_for("Username:", &userinfo.username, 23, text_x, text_y, 0);
    text_y = prompt_for("Password:", &userinfo.passwd, 31, text_x, text_y, 1);
    if(!userinfo.username[0] || !userinfo.passwd[0]) return false;
    un_len = strlen(userinfo.username) + 1; // get length of username (plus terminating)
    pass_len = strlen(userinfo.passwd) + 1; // get length of passwd (plus terminating)
    packet_len = un_len + pass_len + 1; // add buffer lengths and control byte => packet size
    srl_Write(&srl, &packet_len, 3);     // write packet length to srl
    srl_Write(&srl, &ctlcode, 1);           // write LOGIN control byte to srl
    srl_Write(&srl, &userinfo.username, un_len);  // write length bytes from username buffer to srl
    srl_Write(&srl, &userinfo.passwd, pass_len);  // write length bytes from passwd buffer to srl
    // we should be done here?
    return true;
}

bool ntwk_Register(void){
// input = pointer to preserved username/password data from login function
    uint8_t ctlcode = REGISTER;
    uint24_t packet_len;
    uint8_t email_len,un_len, pass_len;

    if(!gameflags.network) return false;

    gfx_ZeroScreen();
    gfx_SetTextFGColor(255);
    gfx_PrintStringXY("User: ", 0, 0);
    gfx_PrintString(userinfo.username);
    gfx_PrintStringXY("No matching account found!", 0, 10);
    gfx_PrintStringXY("Let's create one...", 0, 20);
    gfx_BlitBuffer();
    prompt_for("Email:", &userinfo.email, 63, 0, 30, 0);    // get email address
    if(!userinfo.email[0]) return false;
    email_len = strlen(userinfo.email) + 1;     // get length of email buffer
    un_len = strlen(userinfo.username) + 1; // get length of username (plus terminating)
    pass_len = strlen(userinfo.passwd) + 1; // get length of passwd (plus terminating)
    packet_len = un_len + pass_len + email_len + 1; // add 3 buffers + 1 (control) => packet size
    srl_Write(&srl, &packet_len, 3);                // write size to srl
    srl_Write(&srl, &userinfo.username, un_len);  // write length bytes from username buffer to srl
    srl_Write(&srl, &userinfo.passwd, pass_len);  // write length bytes from passwd buffer to srl
    srl_Write(&srl, &userinfo.email, email_len);                 // write email to srl
    // we should be done here
    return true;
}

void ntwk_Disconnect(void){
    uint8_t ctlcode = DISCONNECT;
    uint24_t packet_len = 1;
    // If we're not connected, there's no reason to disconnect
    if(!gameflags.network) return;

    srl_Write(&srl, &packet_len, 3);
    srl_Write(&srl, &ctlcode, 1);
    // we should be done here
}
