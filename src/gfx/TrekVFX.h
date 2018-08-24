// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekvfx__
#define __trekvfx__
#include <stdint.h>

#include <stdbool.h>

#define trekvfx_num 1

extern uint8_t *trekvfx[1];
#define splashlogo_compressed ((gfx_sprite_t*)trekvfx[0])
bool trekvfx_init(void);

#endif
