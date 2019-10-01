#ifndef gui_h
#define gui_h

extern gfx_rletsprite_t* modicons[TARG_SENS + 1];

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t border, uint8_t bg, uint8_t bar);
void gfx_InitModuleIcons(void);


#endif
