#include <stddef.h>
#include <stdbool.h>
#include <srldrvce.h>
#include <graphx.h>
#include <string.h>
#include "../equates.h"
#include "usb.h"
#include "controlcodes_out.h"
#include "../asm/exposure.h"

uint8_t prompt_for(uint8_t* prompt, uint8_t* buffer, size_t len, x, y){
// returns first newline after prompt
    gfx_PrintStringXY(prompt, x, y);
    gfx_PrintStringXY(">", x, y+8);
    gfx_SetTextXY(x+10, y+8);
    user_input(buffer, len);
    return y+16;
}


void netwk_Login(srl_device_t* srl){
    uint8_t username[24] = {0};
    uint8_t passwd[32] = {0};
    uint8_t ctlcode = LOGIN;
    uint24_t text_x = 0;
    uint8_t text_y = 0;
    uint24_t packet_len;
    uint8t un_len, pass_len;
    gfx_ZeroScreen();
    gfx_SetTextFGColor(228);
    text_y = prompt_for("Username:", &username, 23, text_x, text_y);
    text_y = prompt_for("Password:", &password, 31, text_x, text_y);
    un_len = strlen(&username) + 1; // get length of username (plus terminating)
    pass_len = strlen(&passwd) + 1; // get length of passwd (plus terminating)
    packet_len = un_len + pass_len + 1; // add buffer lengths and control byte => packet size
    srl_Write(srl, &packet_len, 3);     // write packet length to srl
    srl_Write(srl, &ctlcode, 1);           // write LOGIN control byte to srl
    srl_Write(srl, &username, un_len);  // write length bytes from username buffer to srl
    srl_Write(srl, &passwd, pass_len);  // write length bytes from passwd buffer to srl
    // we should be done here?
}
