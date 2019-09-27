
#ifndef modules_h
#define modules_h

#include <stdint.h>
#include <graphx.h>

// Organization for Sprites

enum ModuleTypes {
    LIFESUP,
    CORE,
    WARPDR,
    IMPDR,
    NAVSENS,
    TRANS,
    SYS_MAX,
    SHIELD = SYS_MAX,
    ARMOR,
    PHASER,
    TORPEDO,
    TARG_SENS
};

typedef struct {
    uint24_t id;
    gfx_rletsprite_t* sprite;
    uint8_t name[16];
} moduleindex_t;

extern moduleindex_t moduledb[TARG_SENS];


#endif
