// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekvfx__
#define __trekvfx__
#include <stdint.h>

#include <stdbool.h>

#define trekvfx_num 6

extern uint8_t *trekvfx[6];
#define narada_width 64
#define narada_height 64
#define narada_compressed ((gfx_sprite_t*)trekvfx[0])
#define phaser_width 64
#define phaser_height 64
#define phaser_compressed ((gfx_sprite_t*)trekvfx[1])
#define torp_photon_width 64
#define torp_photon_height 64
#define torp_photon_compressed ((gfx_sprite_t*)trekvfx[2])
#define torp_quantum_width 64
#define torp_quantum_height 64
#define torp_quantum_compressed ((gfx_sprite_t*)trekvfx[3])
#define shield_flare_width 32
#define shield_flare_height 32
#define shield_flare_compressed ((gfx_sprite_t*)trekvfx[4])
#define explosion_width 32
#define explosion_height 32
#define explosion_compressed ((gfx_sprite_t*)trekvfx[5])
bool trekvfx_init(void);

#endif
