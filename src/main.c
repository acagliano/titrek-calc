//--------------------------------------
// Program Name: Project TI-Trek
// Author: Anthony Cagliano
// License: GPL3.0
// Description:
//--------------------------------------

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <intce.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h, debug.h
#include <libload.h>
#include <fileioc.h>
#include <graphx.h>
#include <compression.h>
#include <keypadc.h>
#include <fontlibc.h>
#include <hashlib.h>

#include "graphics/font/trekfont.h"
#include "graphics/menus.h"
#include "core/settings.h"
#include "core/network.h"


int main(void) {

    // init the program
    srandom(rtc_Time());
    
    // init the graphics
    gfx_Begin();
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    fontlib_SetFont(trekfont, 0);
    fontlib_SetTransparency(true);
    fontlib_SetBackgroundColor(0);
    fontlib_SetWindowFullScreen();
    fontlib_SetLineSpacing(1, 1);
    fontlib_SetNewlineOptions(0);
    fontlib_DrawString("init cryptographic rng... ");
    gfx_BlitBuffer();
    if(!csrand_init()) return 1;
    fontlib_DrawString("success\n");
    ntwk_init();
    if(!settings_load()) settings_write();
    
    menu_MainMenu();
    
    // cleanup the program
    gfx_End();
    usb_Cleanup();
}
