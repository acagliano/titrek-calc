#include <stdio.h>
#include <string.h>
#include <graphx.h>
#include <compression.h>
#include <fileioc.h>

#include "gfx/internal.h"
#include "inet/devices.h"
#include "../gamestate.h"
#include "../ev.h"

#define MM_OPTCOUNT 3
const char mm_optstrings[MM_OPTCOUNT][12] = {
	"Play Game",
	"About Game",
	"Exit"
};
const char mm_devicestmp[3][5] = {
	"TCP",
	"SRL"
};


void screendata_init(void){
	gamestate.screendata[SCRN_SPLASH].selected = 0;
	gamestate.screendata[SCRN_SPLASH].num_opts = 3;
}

void frame_render_splash(void);
void frame_render_serverlist(void);
void frame_render_about(void);

void (*screen_render[SCRNS_TOTAL])() = {
	NULL,
	frame_render_splash,
	frame_render_serverlist,
	frame_render_about
};

void frame_screen_up(uint8_t screen_up){
	dequeue(PROC_RENDER, -1);
	enqueue(screen_render[screen_up], PROC_RENDER, true);
	enqueue(gfx_SwapDraw, PROC_RENDER, true);
	gamestate.screen_up = screen_up;
}

#define SPLASH_BORDERCOLOR	19
#define SPLASH_BGCOLOR		190
#define SPLASH_ACTVCOLOR	223
#define CEMU_CONSOLE ((char*)0xFB0000)
#define SPLASH_DEVICE_ID ((gamestate.inet_data.inet_flags>>INET_DEVICE) & 3)
void frame_render_splash(void){
	if (!((gamestate.gameflags>>FRAME_DIRTY) & 1)) return;
	static bool icons_extracted = false;
	static uint8_t icon_usb[15*15+2];
	static uint8_t icon_encrypt[15*15+2];
	if(!icons_extracted){
		zx7_Decompress(icon_usb, icon_usb_compressed);
		zx7_Decompress(icon_encrypt, icon_encrypt_compressed);
		icons_extracted = true;
	}
	gfx_SetTransparentColor(0);
	uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
	uint24_t stringw = gfx_GetStringWidth(VSTRING);
	gfx_ZeroScreen();					// zero screen
	
	// draw header (version string)
	
	gfx_SetColor(224); gfx_FillTriangle(0, 95, 320, 128, 320, 141);
	gfx_SetColor(255); gfx_FillTriangle(0, 95, 320, 131, 320, 138);
	
	gfx_SetColor(SPLASH_BORDERCOLOR);
	gfx_FillRectangle(20, 94, stringw+24, 16);
	gfx_SetTextFGColor(255);
	gfx_SetTextScale(3,3);
	gfx_PrintStringXY("TI-TREK", 165, 85);
	gfx_SetTextScale(1,1);
	gfx_PrintStringXY("A space-combat MMO", 170, 150);
	gfx_PrintStringXY("for your calculator", 170, 160);
	gfx_PrintStringXY("http://titrek.us", 170, 175);
	gfx_PrintStringXY(VSTRING, 32, 98);
	gfx_SetTextFGColor(0);
	// draw footer
	gfx_FillRectangle(20, 191, 130, 16);
	
	// draw 3px border
	gfx_Rectangle(20, 110, 130, 81);
	gfx_Rectangle(21, 111, 128, 79);
	gfx_Rectangle(22, 112, 126, 77);
	// draw menu background
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(23, 113, 124, 75);
	
	// draw menu tabs
	gfx_SetColor(SPLASH_ACTVCOLOR);			// set color for active tab
	gfx_FillRectangle(23, opt_selected*25+112, 124, 25);
	for(int i=0; i<MM_OPTCOUNT; i++){
		gfx_PrintStringXY(mm_optstrings[i], 37, i*25+120);
	}
	
	if((gamestate.inet_data.inet_flags>>INET_ACTIVE) & 1){
		gfx_SetTextFGColor(255);
		gfx_RLETSprite((gfx_rletsprite_t*)icon_usb, 22, 190);
		gfx_PrintStringXY(mm_devicestmp[SPLASH_DEVICE_ID], 38, 194);
	}
	if((gamestate.inet_data.inet_flags>>INET_ENABLE_ENCRYPTION) & 1)
		gfx_RLETSprite((gfx_rletsprite_t*)icon_encrypt, 150-20, 190);
}

void frame_render_about(void){
	static const char *scrn_title = "About TI-Trek";
	uint24_t stringw = gfx_GetStringWidth(scrn_title);
	gamestate.screendata[SCRN_ABOUT].num_opts = 1;
	gfx_ZeroScreen();
	
	gfx_SetColor(SPLASH_BORDERCOLOR);
	gfx_FillRectangle(0, 0, 320, 12);
	
	gfx_SetTextFGColor(255);
	gfx_PrintStringXY(scrn_title, (320-stringw)>>1, 2);
	
	gfx_PrintStringXY("A space-combat MMO for your", 5, 16);
	gfx_PrintStringXY("TI-84+ CE graphing calculator.", 5, 26);
	gfx_PrintStringXY("++++++++", 5, 36);
	gfx_PrintStringXY("Powered by SRL/USB & TCP/USB", 5, 46);
	gfx_PrintStringXY("Encryption via CryptX", 5, 56);
	gfx_PrintStringXY("++++++++", 5, 66);
	gfx_PrintStringXY("Concept by Anthony Cagliano", 5, 76);
	gfx_PrintStringXY("Client/Server Dev:", 5, 86);
		gfx_PrintStringXY("Anthony Cagliano", 150, 86);
		gfx_PrintStringXY("Adam Beckingham", 150, 96);
	gfx_PrintStringXY("Ntwk/Bridge Support: John Caesarz", 5, 106);
	gfx_PrintStringXY("Gfx Support:", 5, 116);
		gfx_PrintStringXY("Bailey Conrad", 150, 116);
		gfx_PrintStringXY("Ampersand", 150, 126);
		gfx_PrintStringXY("Pieman", 150, 136);
		gfx_PrintStringXY("Mike Camardella", 150, 146);
	gfx_PrintStringXY("Astrophysics Support: Bailey Conrad", 5, 156);
	gfx_PrintStringXY("++++++++", 5, 166);
	gfx_PrintStringXY("http://titrek.us", 5, 176);
	
	
}

void frame_render_serverlist(void){
	void *vat_ptr = NULL;
	char *appv_name;
	static const char *scrn_title = "TI-Trek Server Metafiles";
	static const char *prefix_str = "TrekIdentity";
	uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
	uint8_t idx = 0,  i;
	ti_var_t fp;
	uint24_t stringw = gfx_GetStringWidth(scrn_title);
	while((appv_name = ti_Detect(&vat_ptr, prefix_str)))
		strncpy(gamestate.server_identities[idx++], appv_name, 8);
	gamestate.screendata[SCRN_SERVERLIST].num_opts = idx+1;
	gfx_ZeroScreen();
	
	gfx_SetColor(SPLASH_BORDERCOLOR);
	gfx_FillRectangle(0, 0, 320, 12);
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(2, 15, 320-4, 240-15);
	
	gfx_SetTextFGColor(255);
	gfx_PrintStringXY(scrn_title, (320-stringw)>>1, 2);
	gfx_SetTextFGColor(0);
	
	gfx_SetColor(SPLASH_ACTVCOLOR);
	gfx_FillRectangle(2, opt_selected*10+15, 320-4, 10);
	for(i=0; i<idx; i++){
		if ((fp = ti_Open(gamestate.server_identities[i], 'r'))){
			uint8_t *dptr = ti_GetDataPtr(fp);
			dptr += strlen(prefix_str);
			
		} else {
			// error
		}
	}
	gfx_PrintStringXY("Back to main menu", 5, i*10+16);
}
