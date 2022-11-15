#include <stdlib.h>
#include <tice.h>
#include <graphx.h>
#include "../gamestate.h"
#include "../ev.h"
#include "../asm/exposure.h"
#include "frames.h"
#include "../inet/conn.h"

sk_key_t key = 0;

void io_keydetect_menu(void){
	key = getKey();
	if(!key) return;
	uint8_t sel = gamestate.screendata[gamestate.screen_up].selected,
			num_opts = gamestate.screendata[gamestate.screen_up].num_opts;
	if ((key == sk_Enter) || (key == sk_2nd)) {
		switch(gamestate.screen_up){
			case SCRN_SPLASH:
				if (sel == 0) frame_screen_up(SCRN_SERVERLIST);
				else if (sel == 1) frame_screen_up(SCRN_ABOUT);
				else exit(EXIT_OK);
				break;
			case SCRN_SERVERLIST:
				if((sel+1) == num_opts) frame_screen_up(SCRN_SPLASH);
				else {
					frame_screen_up(SCRN_CONSOLE);
					console_insert_line("pinging bridge for status", MSG_NORMAL);
					bridge->ping();
					enqueue(inet_get_packet, PROC_NTWK, true);
				}
				break;
		}
	}
	else if (key == sk_Down) {
		sel++;
		sel %= num_opts;
	} else if (key == sk_Up) {
		sel += (num_opts-1);
		sel %= num_opts;
	} else if (key == sk_Clear) {
		switch(gamestate.screen_up){
			case SCRN_SPLASH:
				exit(EXIT_OK);
				break;
			case SCRN_SERVERLIST:
			case SCRN_ABOUT:
				frame_screen_up(SCRN_SPLASH);
				break;
		}
	}
	gamestate.screendata[gamestate.screen_up].selected = sel;
	MARK_FRAME_DIRTY;
}
