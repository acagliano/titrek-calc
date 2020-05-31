
#ifndef modules_h
#define modules_h

#include <stdint.h>
#include <graphx.h>

// Organization for Sprites
#define MODICONS 14     // define first module icon

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

enum ModuleClasses {
    mUnassigned = 0,
    mSystem,
    mTactical
};


#endif
