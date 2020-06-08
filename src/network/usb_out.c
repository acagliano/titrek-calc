#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <srldrvce.h>
#include <graphx.h>
#include <string.h>
#include "../equates.h"
#include "usb.h"
#include "controlcodes_out.h"
#include "../asm/exposure.h"

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y){
// returns first newline after prompt
    gfx_PrintStringXY(prompt, x, y);
    gfx_PrintStringXY(">", x, y+8);
    gfx_SetTextXY(x+10, y+8);
    user_input(buffer, len);
    return y+16;
}


void ntwk_Login(void){
    char username[24] = {0};
    char passwd[32] = {0};
    uint8_t ctlcode = LOGIN;
    uint24_t text_x = 0;
    uint8_t text_y = 0;
    uint24_t packet_len;
    uint8_t un_len, pass_len;
    gfx_ZeroScreen();
    gfx_SetTextFGColor(228);
    text_y = prompt_for("Username:", &username, 23, text_x, text_y);
    text_y = prompt_for("Password:", &passwd, 31, text_x, text_y);
    un_len = strlen(&username) + 1; // get length of username (plus terminating)
    pass_len = strlen(&passwd) + 1; // get length of passwd (plus terminating)
    packet_len = un_len + pass_len + 1; // add buffer lengths and control byte => packet size
    srl_Write(&srl, &packet_len, 3);     // write packet length to srl
    srl_Write(&srl, &ctlcode, 1);           // write LOGIN control byte to srl
    srl_Write(&srl, &username, un_len);  // write length bytes from username buffer to srl
    srl_Write(&srl, &passwd, pass_len);  // write length bytes from passwd buffer to srl
    // we should be done here?
}

void ntwk_Register(uint8_t* loginstuff, size_t buff_size){
// input = pointer to preserved username/password data from login function
    char email[64] = {0};
    uint8_t ctlcode = REGISTER;
    uint24_t packet_len;
    uint8_t email_len;
    gfx_PrintStringXY("No matching account found!", 0, 0);
    gfx_PrintStringXY("Let's create one...", 0, 8);
    prompt_for("Email:", &email, 63, 0, 16);    // get email address
    email_len = strlen(&email) + 1;     // get length of email buffer
    packet_len = buff_size + email_len + 1; // add 3 buffers + 1 (control) => packet size
    srl_Write(&srl, &packet_len, 3);                // write size to srl
    srl_Write(&srl, &loginstuff, buff_size);    // write loginstuff to srl
    srl_Write(&srl, &email, email_len);                 // write email to srl
    // we should be done here
}

void ntwk_Disconnect(void){
    uint8_t ctlcode = DISCONNECT;
    uint24_t packet_len = 1;
    srl_Write(&srl, &packet_len, 3);
    srl_Write(&srl, &ctlcode, 1);
    // we should be done here
}
