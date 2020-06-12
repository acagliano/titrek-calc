#include <tice.h>
#include <graphx.h>
#include <stddef.h>
#include <compression.h>
#include "gfx/internal.h"
#include "network/controlcodes.h"
#include "equates.h"

#define center_x (320/2)
#define center_y (180/2)
#define widget_width 200
#define widget_height 39
#define widget_x (center_x - (widget_width/2))
#define widget_y (center_y - (widget_height/2))

char ctl_codes[][12] = {
    "Register",
    "Login",
    "Disconnect",
    "Status"
};

char err_codes[][32] = {
    "Auth Invalid",
    "Duplicate",
    "Missing",      // placeholder
    "Banned",
    "Version Err",
    "Logging you in...",
    "Packet Received"
};

debug_type[][10] = {
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

void gui_NetworkErrorResponse(uint8_t controlcode, uint8_t responsecode){
    gfx_DrawErrorWidget();
    gfx_SetTextXY(widget_x + 50, widget_y + 8);
    gfx_PrintString(ctl_codes[controlcode]);
    gfx_SetTextXY(widget_x + 50, widget_y + 18);
    gfx_PrintString(err_codes[responsecode - 1]);
}

