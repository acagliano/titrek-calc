#include <stdlib.h>
#include <tice.h>
#include <graphx.h>
#include "../gamestate.h"
#include "../ev.h"
#include "../asm/exposure.h"
#include "frames.h"

sk_key_t key = 0;

void io_keydetect_menu(void){
	key = getKey();
	if(!key) return;
	uint8_t sel = gamestate.screendata[gamestate.screen_up].selected,
			num_opts = gamestate.screendata[gamestate.screen_up].num_opts;
	if ((key == sk_Enter) || (key == sk_2nd)) {
		dequeue(PROC_RENDER, -1);
		switch(gamestate.screen_up){
			case SCRN_SPLASH:
				if (sel == 0) {
					enqueue(frame_render_serverlist, PROC_RENDER, true);
					enqueue(gfx_SwapDraw, PROC_RENDER, true);
					gamestate.screen_up = SCRN_SERVERLIST;
				}
				else if (sel == 1) {
					//enqueue(frame_render_settings, PROC_RENDER, true);
					//enqueue(io_keydetect_settings, PROC_KEYDETECT, true);
					gamestate.screen_up = SCRN_SETTINGS;
				}
				else exit(EXIT_OK);
				break;
			case SCRN_SERVERLIST:
				if((sel+1) == num_opts){
					enqueue(frame_render_splash, PROC_RENDER, true);
					enqueue(gfx_SwapDraw, PROC_RENDER, true);
					gamestate.screen_up = SCRN_SPLASH;
				}
				else {
					// not implemented
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
	} else if (key == sk_Clear) exit(EXIT_OK);
	gamestate.screendata[gamestate.screen_up].selected = sel;
	MARK_FRAME_DIRTY;
}
