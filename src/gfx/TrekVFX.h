// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __TrekVFX__
#define __TrekVFX__
#include <stdint.h>

#include <stdbool.h>

#define TrekVFX_num 1

extern uint8_t *TrekVFX[1];
#define enterprise_compressed ((gfx_sprite_t*)TrekVFX[0])
bool TrekVFX_init(void);

#endif
