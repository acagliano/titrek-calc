#include <stdlib.h>
#include <tice.h>
#include <graphx.h>
#include "../gamestate.h"
#include "../ev.h"
#include "../asm/exposure.h"
#include "frames.h"

sk_key_t key = 0;
void io_keydetect_serverlist(void);

void io_keydetect_splash(void){
	key = getKey();
	if(!key) return;
	uint8_t sel = gamestate.screendata[gamestate.screen_up].selected,
			num_opts = gamestate.screendata[gamestate.screen_up].num_opts;
	if ((key == sk_Enter) || (key == sk_2nd)) {
		dequeue(PROC_KEYDETECT, -1);
		dequeue(PROC_RENDER, -1);
		if (sel == 0) {
			enqueue(frame_render_serverlist, PROC_RENDER, true);
			enqueue(gfx_SwapDraw, PROC_RENDER, true);
			enqueue(io_keydetect_serverlist, PROC_KEYDETECT, true);
			gamestate.screen_up = SCRN_SERVERLIST;
		}
		else if (sel == 1) {
			//enqueue(frame_render_settings, PROC_RENDER, true);
			//enqueue(io_keydetect_settings, PROC_KEYDETECT, true);
			gamestate.screen_up = SCRN_SETTINGS;
		}
		else exit(EXIT_OK);
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

void io_keydetect_serverlist(void){
	key = getKey();
	if(!key) return;
	uint8_t sel = gamestate.screendata[gamestate.screen_up].selected,
	num_opts = gamestate.screendata[gamestate.screen_up].num_opts + 1;
	if ((key == sk_Enter) || (key == sk_2nd) || (key == sk_Clear)) {
		dequeue(PROC_KEYDETECT, -1);
		dequeue(PROC_RENDER, -1);
		if(sel == (num_opts-1)){
			enqueue(frame_render_splash, PROC_RENDER, true);
			enqueue(gfx_SwapDraw, PROC_RENDER, true);
			enqueue(io_keydetect_splash, PROC_KEYDETECT, true);
			gamestate.screen_up = SCRN_SPLASH;
		}
		else {
			// not implemented
		}
	}
	else if (key == sk_Down) {
		sel++;
		sel %= num_opts;
	} else if (key == sk_Up) {
		sel += (num_opts-1);
		sel %= num_opts;
	}
	gamestate.screendata[gamestate.screen_up].selected = sel;
	MARK_FRAME_DIRTY;
}
