#include <stdio.h>
#include <graphx.h>
#include <compression.h>

#include "gfx/internal.h"
#include "inet/devices.h"
#include "../gamestate.h"

#define MM_OPTCOUNT 3
const char mm_optstrings[MM_OPTCOUNT][12] = {
	"Play Game",
	"Settings",
	"Exit"
};


void screendata_init(void){
	gamestate.screendata[SCRN_SPLASH].selected = 0;
	gamestate.screendata[SCRN_SPLASH].num_opts = 3;
}


#define SPLASH_BORDERCOLOR	19
#define SPLASH_BGCOLOR		157
#define SPLASH_ACTVCOLOR	190
#define CEMU_CONSOLE ((char*)0xFB0000)
void frame_render_splash(void){
	if (!((gamestate.gameflags>>FRAME_DIRTY) & 1)) return;
	static bool icons_extracted = false;
	static uint8_t icon_usb[16*16+2];
	uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
	uint24_t stringw = gfx_GetStringWidth(VSTRING);
	gfx_ZeroScreen();					// zero screen
	
	// draw header (version string)
	gfx_SetColor(SPLASH_BORDERCOLOR);
	gfx_FillRectangle(30, 94, stringw+24, 16);
	gfx_SetTextFGColor(255); gfx_PrintStringXY(VSTRING, 42, 98);
	gfx_SetTextFGColor(0);
	// draw footer
	gfx_FillRectangle(30, 191, 130, 18);
	
	// draw 3px border
	gfx_Rectangle(30, 110, 130, 81);
	gfx_Rectangle(31, 111, 128, 79);
	gfx_Rectangle(32, 112, 126, 77);
	// draw menu background
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(33, 113, 124, 75);
	
	// draw menu tabs
	gfx_SetColor(SPLASH_ACTVCOLOR);			// set color for active tab
	for(int i=0; i<MM_OPTCOUNT; i++){
		if(i == opt_selected)
			gfx_FillRectangle(33, i*25+112, 124, 25);
		gfx_PrintStringXY(mm_optstrings[i], 37, i*25+120);
	}
	
	if(!icons_extracted){
		zx7_Decompress(icon_usb, icon_usb_compressed);
		icons_extracted = true;
	}
	sprintf(CEMU_CONSOLE, "inet up: %u\n", (gamestate.inet_data.inet_flags>>INET_ACTIVE) & 1);
	if((gamestate.inet_data.inet_flags>>INET_ACTIVE) & 1)
		gfx_RLETSprite((gfx_rletsprite_t*)icon_usb, 32, 193);
	gfx_BlitBuffer();
}
