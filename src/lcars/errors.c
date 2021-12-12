#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <stddef.h>
#include <compression.h>
#include <string.h>
#include "../gfx/internal.h"
#include "../network/controlcodes.h"
#include "../classes/settings.h"
#include "../equates.h"
#include "errors.h"
#include "text.h"

#define center_x (320/2)
#define center_y (180/2)
#define widget_width 200
#define widget_height 39
#define widget_x (center_x - (widget_width/2))
#define widget_y (center_y - (widget_height/2))

char log_msg[4][LOG_LINE_SIZE] = {0};
gfx_rletsprite_t* icon[4] = {NULL};
uint24_t log_display[4] = {0};
char err_text[][25] = {"", "ERR: Gfx", "ERR: Network", "Err: Bridge", "Err: Timeout", "Err: No Login Token"};

char ctl_codes[][20] = {
    "CONNECT",
    "DISCONNECT",
    "LOGIN"
    "REGISTER",
    "LOGOUT",
    "VERSION_ERR"
};

char err_codes[][32] = {
    "Success",
    "Invalid",
    "Duplicate",
    "Missing",      // placeholder
    "User Banned",
    "Incompatible",
    "Outdated",
    "Packet Format",
    "Network Timeout"
};



char debug_type[][10] = {
    "srl_recv"
};

void gfx_DrawErrorWidget(void){
    gfx_SetColor(96);
    gfx_Rectangle(widget_x, widget_y, widget_width, widget_height);
    gfx_Rectangle(widget_x+1, widget_y+1, widget_width-2, widget_height-2);
    gfx_SetColor(238);
    gfx_FillRectangle(widget_x+2, widget_y+2, widget_width-4, widget_height-4);
    gfx_RLETSprite(err_icon, widget_x+5, widget_y+7);
}

void gui_NetworkErrorResponse(uint8_t controlcode, uint8_t responsecode, bool blocking){
    gfx_DrawErrorWidget();
    gfx_SetTextXY(widget_x + 50, widget_y + 8);
    gfx_PrintString(ctl_codes[controlcode]);
    gfx_SetTextXY(widget_x + 50, widget_y + 18);
    gfx_PrintString(err_codes[responsecode]);
    if(blocking){
        gfx_BlitBuffer();
        while(!kb_IsDown(kb_KeyClear)) kb_Scan();
    }
}

void gui_SetLog(uint8_t type, const char* msg){
    uint8_t i;
    for(i = 3; i > 0; i--){
        strncpy(&log_msg[i], &log_msg[i-1], LOG_LINE_SIZE);
        icon[i] = icon[i-1];
        log_display[i] = log_display[i-1];
    }
    memset(&log_msg[0], 0, LOG_LINE_SIZE);
    strncpy(&log_msg[0], msg, LOG_LINE_SIZE);
    log_display[0] = settings.limits.log_limit;
    switch(type){
        case LOG_ERROR:
            icon[0] = log_error;
            break;
        case LOG_INFO:
            icon[0] = log_info;
            break;
        case LOG_DEBUG:
            icon[0] = log_debug;
            break;
        case LOG_SERVER:
            icon[0] = log_server;
            break;
    }
}

void gui_ShowLog(void){
    uint8_t i, h = 0;
    uint24_t x = 2;
    uint8_t y = 216-2;
    for(i=0; i<4; i++){
        if(!log_display[i]) {memset(&log_msg[i], 0, LOG_LINE_SIZE); full_redraw = true;}
        if(*log_msg[i])
            h+=text_GetStringHeight(log_msg[i], 312-x+12);
    }
    h*=10;
    h+=6;
    gfx_SetColor(163); gfx_Rectangle(x, y-h, 320-4, h);
    gfx_SetColor(0); gfx_FillRectangle(x+1, y+1-h, 320-6, h-2);
    x+=5; y-=6;
    for(i=0; i<4;i++){
        if(*log_msg[i]){
            log_display[i]=log_display[i]-1;
            y-=(10*text_GetStringHeight(log_msg[i], 312-x+12));
            gfx_RLETSprite(icon[i], x, y+3);
            gfx_SetTextFGColor(255);
            text_WrappedString(log_msg[i], x+12, y+4, 312);
        }
    }
}
