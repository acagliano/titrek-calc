// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekvfx__
#define __trekvfx__
#include <stdint.h>

#include <stdbool.h>

#define trekvfx_num 2

extern uint8_t *trekvfx[2];
#define target_width 30
#define target_height 30
#define target_compressed ((gfx_sprite_t*)trekvfx[0])
#define phaser_width 32
#define phaser_height 32
#define phaser_compressed ((gfx_sprite_t*)trekvfx[1])
bool trekvfx_init(void);

#endif
