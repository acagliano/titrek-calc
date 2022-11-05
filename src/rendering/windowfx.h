#ifndef windowfx_h
#define windowfx_h

#include <stdint.h>

void fx_renderwindow(uint24_t x, uint8_t y, uint24_t w, uint8_t h,
					 uint8_t bgcol, uint8_t bordercol, uint8_t borderw,
					 const char *title);


#endif
