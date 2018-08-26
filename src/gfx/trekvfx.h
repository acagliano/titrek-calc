// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekvfx__
#define __trekvfx__
#include <stdint.h>

#include <stdbool.h>

#define trekvfx_num 4

extern uint8_t *trekvfx[4];
#define target_width 30
#define target_height 30
#define target_compressed ((gfx_sprite_t*)trekvfx[0])
#define phaser_width 32
#define phaser_height 32
#define phaser_compressed ((gfx_sprite_t*)trekvfx[1])
#define torp_photon_width 32
#define torp_photon_height 32
#define torp_photon_compressed ((gfx_sprite_t*)trekvfx[2])
#define torp_quantum_width 32
#define torp_quantum_height 32
#define torp_quantum_compressed ((gfx_sprite_t*)trekvfx[3])
bool trekvfx_init(void);

#endif
