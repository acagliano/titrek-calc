#include <tice.h>
#include <graphx.h>
#include <stddef.h>
#include "network/controlcodes.h"

#define center_x (320/2)
#define center_y (240/2)
#define widget_width 100
#define widget_height 44
#define widget_x (center_x - (widget_width/2))
#define widget_y (center_y - (widget_height/2))

char ctl_codes[][12] = {
    "Register",
    "Login",
    "Disconnect",
    "Status"
};

char err_codes[][20] = {
    "Auth Invalid",
    "Duplicate",
    "Missing",      // placeholder
    "Banned",
    "Version Err"
    "Logging you in..."
};

void gfx_DrawErrorWidget(void){
    gfx_SetColor(96);
    gfx_Rectangle(widget_x, widget_y, widget_width, widget_height);
    gfx_Rectangle(widget_x+1, widget_y+1, widget_width-2, widget_height-2);
    gfx_SetColor(238);
    gfx_FillRectangle(widget_x+2, widget_y+2, widget_width-4, widget_height-4);
}

void gui_NetworkErrorResponse(uint8_t controlcode, uint8_t responsecode){
    gfx_DrawErrorWidget();
    gfx_SetTextXY(widget_x + 40, widget_y + 8);
    gfx_PrintString(ctl_codes[controlcode]);
    gfx_SetTextXY(widget_x + 40, widget_y + 18);
    gfx_PrintString(err_codes[responsecode - 1]);
    gfx_BlitBuffer();
}
