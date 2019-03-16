// convpng v7.0
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekgui__
#define __trekgui__
#include <stdint.h>

#include <stdbool.h>

#define trekgui_num 1

extern uint8_t *trekgui[1];
#define shipinterior_width 160
#define shipinterior_height 240
#define shipinterior_compressed ((gfx_sprite_t*)trekgui[0])
bool trekgui_init(void);

#endif
