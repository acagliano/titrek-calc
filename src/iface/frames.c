#include <stdio.h>
#include <string.h>
#include <graphx.h>
#include <compression.h>
#include <fileioc.h>

#include "gfx/internal.h"
#include "inet/inet.h"
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
void frame_render_console(void);

void (*screen_render[SCRNS_TOTAL])() = {
	NULL,
	frame_render_splash,
	frame_render_serverlist,
	frame_render_about,
	frame_render_console
};

void frame_screen_up(uint8_t screen_up){
	dequeue(PROC_RENDER, -1);
	enqueue(screen_render[screen_up], PROC_RENDER, true);
	gamestate.screen_up = screen_up;
	MARK_FRAME_DIRTY;
}

#define SPLASH_BORDERCOLOR	19
#define SPLASH_TITREKCOLOR	20
#define SPLASH_BGCOLOR		190
#define SPLASH_ACTVCOLOR	223
void frame_render_headerbar(const char *title){
	uint24_t stringw = gfx_GetStringWidth(title);
	gfx_SetColor(SPLASH_BORDERCOLOR);
	gfx_FillRectangle(0, 0, 320, 12);
	
	gfx_SetTextFGColor(255);
	gfx_PrintStringXY(title, (320-stringw)>>1, 2);
}

#define CEMU_CONSOLE ((char*)0xFB0000)
void frame_render_splash(void){
	if(!GET_FLAG(gamestate.gameflags, FRAME_DIRTY)) return;
	static bool splash_icons_extracted = false;
	static uint8_t icon_usb[15*15+2];
	static uint8_t icon_usb_error[15*15+2];
	const char *gamename = "TI-TREK";
//	static uint8_t icon_encrypt[15*15+2];
	if(!splash_icons_extracted){
		zx7_Decompress(icon_usb, icon_usb_compressed);
		zx7_Decompress(icon_usb_error, icon_usb_error_compressed);
		//zx7_Decompress(icon_encrypt, icon_encrypt_compressed);
		splash_icons_extracted = true;
	}
	gfx_SetTransparentColor(0);
	uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
	uint24_t stringw = gfx_GetStringWidth(VSTRING);
	gfx_ZeroScreen();					// zero screen
	
	// draw header (version string)
	
	gfx_SetColor(224); gfx_FillTriangle(0, 65, 320, 128, 320, 141);
	gfx_SetColor(255); gfx_FillTriangle(0, 65, 320, 131, 320, 138);
	gfx_SetTextFGColor(255);
	gfx_SetColor(SPLASH_BORDERCOLOR);
	
	// version string bg
	gfx_FillRectangle(20, 104, stringw+24, 16);
	
	// TI-TREK text
	gfx_FillRectangle(90, 20, 320-110-5, 40);
	gfx_SetTextScale(4,4);
	gfx_PrintStringXY(gamename, 95, 25);
	gfx_SetTextScale(1,1);
	
	gfx_PrintStringXY("A space-combat MMO", 170, 150);
	gfx_PrintStringXY("for your calculator", 170, 160);
	gfx_PrintStringXY("http://titrek.us", 170, 175);
	gfx_PrintStringXY(VSTRING, 32, 108);
	gfx_SetTextFGColor(0);
	// draw footer
	gfx_FillRectangle(20, 201, 130, 16);
	
	// draw 3px border
	gfx_Rectangle(20, 120, 130, 81);
	gfx_Rectangle(21, 121, 128, 79);
	gfx_Rectangle(22, 122, 126, 77);
	// draw menu background
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(23, 123, 124, 75);
	
	// draw menu tabs
	gfx_SetColor(SPLASH_ACTVCOLOR);			// set color for active tab
	gfx_FillRectangle(23, opt_selected*25+122, 124, 25);
	for(int i=0; i<MM_OPTCOUNT; i++){
		gfx_PrintStringXY(mm_optstrings[i], 37, i*25+130);
	}
	
	if(GET_FLAG(gamestate.inet.flags, INET_ACTIVE)){
		gfx_SetTextFGColor(255);
		gfx_RLETSprite((gfx_rletsprite_t*)icon_usb, 25, 201);
		gfx_PrintStringXY(mm_devicestmp[gamestate.inet.device_id], 43, 205);
	}
	else gfx_RLETSprite((gfx_rletsprite_t*)icon_usb_error, 25, 201);
	gfx_SwapDraw();
	MARK_FRAME_CLEAN;
}

void frame_render_about(void){
	if(!GET_FLAG(gamestate.gameflags, FRAME_DIRTY)) return;
	static const char *scrn_title = "About TI-Trek";
	gamestate.screendata[SCRN_ABOUT].num_opts = 1;
	gfx_ZeroScreen();
	
	frame_render_headerbar(scrn_title);
	
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
	gfx_PrintStringXY("Ntwk/Bridge Support: John Cesarz", 5, 106);
	gfx_PrintStringXY("Gfx Support:", 5, 116);
		gfx_PrintStringXY("Bailey Conrad", 150, 116);
		gfx_PrintStringXY("Ampersand", 150, 126);
		gfx_PrintStringXY("Pieman", 150, 136);
		gfx_PrintStringXY("Mike Camardella", 150, 146);
	gfx_PrintStringXY("Astrophysics Support: Bailey Conrad", 5, 156);
	gfx_PrintStringXY("++++++++", 5, 166);
	gfx_PrintStringXY("http://titrek.us", 5, 176);
	gfx_SwapDraw();
	MARK_FRAME_CLEAN;
	
}

void frame_render_serverlist(void){
	if(!GET_FLAG(gamestate.gameflags, FRAME_DIRTY)) return;
	void *vat_ptr = NULL;
	char *appv_name;
	static const char *scrn_title = "TI-Trek Server Metafiles";
	static const char *prefix_str = "TrekIdentity";
	static uint8_t icon_user[8*8+2];
	static bool serverlist_icons_extracted = false;
	if(!serverlist_icons_extracted){
		zx7_Decompress(icon_user, icon_user_compressed);
		serverlist_icons_extracted = true;
	}
	uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
	uint8_t idx = 0,  i;
	ti_var_t fp;
	while((appv_name = ti_Detect(&vat_ptr, prefix_str)))
		strncpy(gamestate.server_identities[idx++], appv_name, 8);
	gamestate.screendata[SCRN_SERVERLIST].num_opts = idx+1;
	gfx_ZeroScreen();
	
	frame_render_headerbar(scrn_title);
	
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(2, 15, 320-4, 240-15);
	gfx_SetTextFGColor(0);
	
	gfx_SetColor(SPLASH_ACTVCOLOR);
	gfx_FillRectangle(2, opt_selected*10+15, 320-4, 10);
	for(i=0; i<idx; i++){
		if ((fp = ti_Open(gamestate.server_identities[i], "r"))){
			uint24_t x = 5;
			uint8_t y = i*10+16;
			uint8_t *dptr = ti_GetDataPtr(fp);
			dptr += strlen(prefix_str);
			gfx_SetTextXY(x, y);
			gfx_PrintString(dptr);
			x = 200;
			dptr += strlen(dptr)+1;
			gfx_RLETSprite((gfx_rletsprite_t*)icon_user, x, y);
			x+=12;
			gfx_SetTextXY(x, y);
			gfx_PrintString(dptr);
			ti_Close(fp);
		} else gfx_PrintStringXY("error opening metafile", 5, i*10+16);
	}
	gfx_PrintStringXY("Back to main menu", 5, i*10+16);
	gfx_SwapDraw();
	MARK_FRAME_CLEAN;
}


#define CONSOLE_LINECT	22
#define CONSOLE_LINEW	32
uint8_t console_buf[CONSOLE_LINECT][CONSOLE_LINEW+2] = {0};
uint8_t console_start_idx = 0;
uint8_t console_stop_idx = 0;


void frame_render_console(void){
	if(!GET_FLAG(gamestate.gameflags, FRAME_DIRTY)) return;
	
	static const char *scrn_title = "Console";
	uint8_t line_y = 16;
	frame_render_headerbar(scrn_title);
	gfx_SetColor(SPLASH_BGCOLOR);
	gfx_FillRectangle(5, 15, 320-10, 240-15);
	
	for(uint8_t i = console_start_idx; i != console_stop_idx; i = ((i+1)%22)){
		uint8_t *ln = &console_buf[console_stop_idx];
		gfx_SetTextFGColor(ln[0]);
		gfx_PrintStringXY(&ln[1], 10, line_y);
		line_y += 10;
	}
	gfx_SwapDraw();
	MARK_FRAME_CLEAN;
}

#define MIN(x, y) (x < y ? x : y)
void console_insert_line(const char *line, uint8_t msg_type){
	size_t len = strlen(line);
	for(size_t i = 0; i < len;){
		size_t copylen = MIN(len-i, CONSOLE_LINEW);
		uint8_t *ln = &console_buf[console_stop_idx];
		ln[0] = msg_type;
		memcpy(&ln[1], line+i, copylen);
		ln[copylen+1] = 0;
		i += copylen;
		console_stop_idx++;
		console_stop_idx %= CONSOLE_LINECT;
		if(console_stop_idx==console_start_idx){
			console_start_idx++;
			console_start_idx %= CONSOLE_LINECT;
		}
	}
}
