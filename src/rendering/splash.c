#include <stdbool.h>
#include <graphx.h>
#include <tice.h>
#include <compression.h>
#include "windowfx.h"
#include "../ev.h"
#include "../gfx/internal.h"

#define MENU_OPTS_COUNT 3
const char opt_strings[MENU_OPTS_COUNT][12] = {
	"Play Game",
	"Settings",
	"Exit"
}


struct starfield_obj {uint24_t x, uint8_t y, uint8_t size};
#define STARFIELD_MAX_STARS	50
void fx_renderstarfield(void){
	static bool starfield_generated = false, star_sprites_extracted = false;
	static struct starfield_obj starfield[STARFIELD_MAX_STARS];
	static uint8_t star_sprites[3][64+2];
	if (!starfield_generated){
		for(int i; i<STARFIELD_MAX_STARS; i++){
			uint32_t r = random();
			starfield[i].x = (uint24_t)(r&0xffffff);
			starfield[i].y = (uint8_t)((r>>24)&0xff);
			starfield[i].size = random() % 3;
		}
		starfield_generated = true;
	}
	if (!star_sprites_extracted){
		zx0_Decompress(star_sprites[0], star_small_compressed);
		zx0_Decompress(star_sprites[1], star_medium_compressed);
		zx0_Decompress(star_sprites[2], star_large_compressed);
		star_sprites_extracted = true;
	}
	for(int i; i<STARFIELD_MAX_STARS; i++)
		gfx_RLETSprite(star_sprites[starfield[i].size], starfield[i].x, starfield[i].y);
	gamestate.gameflags |= (1<<FRAME_DIRTY);
}

void fx_rendersplashmenuopt(const char* opttext, uint24_t x, uint24_t y, bool active){
	uint8_t color = (active==true) ? 107 : 148;
	gfx_SetColor(color);
	gfx_FillRectangle(x, y, 120, 10);
	gfx_PrintString(opttext, x+5, y+2);
}

void render_splashscreen(void){
	static uint8_t menuopt_selected = 0;
	gfx_ZeroScreen();
	fx_renderstarfield();
	fx_renderwindow(100, 70, 120, 100, 148, 195, 2, "Main Menu");
	for(int i = 0; i < MENU_OPTS_COUNT; i++)
		fx_rendersplashmenuopt(opt_strings[i] ,102, 10 * i + 122, (i==menuopt_selected));
}
