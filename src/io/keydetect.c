#include <stdlib.h>
#include <tice.h>
#include "../gamestate.h"
#include "../asm/exposure.h"

sk_key_t key = 0;


void io_keydetect(void){
	key = getKey();
	if (key){
		uint8_t sel = gamestate.screendata[gamestate.screen_up].selected,
				num_opts = gamestate.screendata[gamestate.screen_up].num_opts;
		bool _skip = false;
		switch(gamestate.screen_up){
			case SCRN_NONE:
				break;
			case SCRN_SPLASH:
				if ((key == sk_Enter) || (key == sk_2nd)) {
					if (sel == 0) gamestate.screen_up = SCRN_SERVERLIST;
					else if (sel == 1) gamestate.screen_up = SCRN_SETTINGS;
					else exit(EXIT_OK);
				}
				_skip = true;
			case SCRN_SERVERLIST:
				if (!_skip){
					//placehold
				}
			case SCRN_SETTINGS:
				if (!_skip){
					//placehold
				}
			default:
				if (key == sk_Down) {
					sel++;
					sel %= num_opts;
				} else if (key == sk_Up) {
					sel += (num_opts-1);
					sel %= num_opts;
				} else if (key == sk_Clear) exit(EXIT_OK);
		}
		gamestate.screendata[gamestate.screen_up].selected = sel;
		gamestate.gameflags |= (1<<FRAME_DIRTY);
	}
}
