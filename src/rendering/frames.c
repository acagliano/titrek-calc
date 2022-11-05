
#include "../gamestate.h"

void frame_render(void){
	switch(gamestate.screen_up){
		case SCRN_SPLASH:
			render_splashscreen();
			break;
			
		default:
			
	}
}
