
#ifndef tech_h
#define tech_h

// Enumerate techtypes, classes for module.h and ships.h

enum TechTypes {
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
    TARG_SENS,
    HULL = 0xf0
};

enum TechClasses {
    mUnassigned = 0,
    mSystem,
    mTactical
};


// Module States
#define STATE_OFFLINE 0
#define STATE_ONLINE 1
#define STATE_REPAIR 2

#endif
