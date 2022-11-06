#include <stdint.h>
#include <graphx.h>

void fx_renderwindow(uint24_t x, uint8_t y, uint24_t w, uint8_t h,
					 uint8_t bgcol, uint8_t bordercol, uint8_t borderw,
					 const char *title){
	uint8_t i;
	gfx_SetColor(bordercol);
	if(title!=NULL){
		gfx_FillRectangle(x, y, w, 10);
		gfx_PrintStringXY(title, x+5, y+2);
		y+=10;
	}
	for(i=0; i<borderw; i++)
		gfx_Rectangle(x+i, y+i, w-(i<<1), h-(i<<1));
	gfx_SetColor(bgcol);
	gfx_FillRectangle(x+i, y+i, w-(i<<1), h-(i<<1));
}
