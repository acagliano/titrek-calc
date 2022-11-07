
#include <graphx.h>

#include "../gamestate.h"
#include "windowfx.h"

#define MM_OPTCOUNT 3
const char mm_optstrings[MM_OPTCOUNT][12] = {
	"Play Game",
	"Settings",
	"Exit"
};

void frame_render(void){
	if ((gamestate.gameflags>>FRAME_DIRTY) & 1){
		uint8_t opt_selected = gamestate.screendata[gamestate.screen_up].selected;
		switch(gamestate.screen_up){
			case SCRN_SPLASH:
				gfx_ZeroScreen();
				fx_renderwindow(100, 70, 120, 100, 148, 195, 2, "Main Menu");
				for(int i=0; i<MM_OPTCOUNT; i++){
					//fx_rendersplashmenuopt(opt_strings[i] ,102, 10 * i + 122, (i==menuopt_selected));
					uint8_t color = (i==opt_selected) ? 107 : 148;
					gfx_SetColor(color);
					gfx_FillRectangle(102, i*10+72, 120-4, 10);
					gfx_PrintStringXY(mm_optstrings[i], 107, i*10+74);
				}
				break;
				
		}
	}
}
