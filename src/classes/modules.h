
#ifndef modules_h
#define modules_h

#include <stdint.h>
#include <graphx.h>

// Organization for Sprites
#define MODICONS 14     // define first module icon

//System Structs
typedef struct { unsigned char drv; } integ_data_t;
typedef struct { int occupancy; } lifesup_data_t;
typedef struct { int output; } core_data_t;
typedef struct { int maxspeed; } engine_data_t;
typedef struct { int maxrange; } navsens_data_t;
typedef struct { int maxrange; int maxtargets; } trans_data_t;

//Tactical Structs
typedef struct { int maxrange; char maxtargets; } targsens_data_t;
typedef struct { unsigned int resistance;  int frequency; int maxhealth; int shieldhealth[SHIP_ALL]; } shield_data_t;
typedef struct { int yield; int range; int speed; } phaser_data_t;
typedef struct { int equipped; unsigned char compatible[10]; } torpedo_data_t;
// This union combines all tactical modules into 1

typedef union ModuleInfo_t {
    // Core System Modules
    trans_data_t mod_transport;
    integ_data_t mod_integ;
    lifesup_data_t mod_lifesupport;
    core_data_t mod_core;
    engine_data_t mod_engine;
    navsens_data_t mod_navsens;
    // Shield Modules
    shield_data_t mod_shields;
    // Weapons Modules
    torpedo_data_t mod_torpedoes;
    phaser_data_t mod_phasers;
    targsens_data_t mod_targsens;
} moduleinfo_t;

#endif
