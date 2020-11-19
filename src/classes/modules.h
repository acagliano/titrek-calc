
#ifndef modules_h
#define modules_h

#include <stdint.h>
#include <graphx.h>
#include "health.h"
#include "power.h"
#include "systems/engines.h"

// Organization for Sprites
enum ShipZones {
    FORWARD,
    STARBOARD,
    AFT,
    PORT,
    DORSAL,
    VENTRAL,
    SHIP_ALL
};

//System Structs
typedef struct { unsigned char drv; } integ_data_t;
typedef struct { int occupancy; } lifesup_data_t;
typedef struct { int output; } core_data_t;
typedef struct { int maxrange; } navsens_data_t;
typedef struct { int maxrange; int maxtargets; } trans_data_t;

//Tactical Structs
typedef struct { int maxrange; char maxtargets; } targsens_data_t;
typedef struct { unsigned int resistance;  int frequency; int maxhealth; int shieldhealth[SHIP_ALL]; } shield_data_t;
typedef struct { int yield; int range; int speed; } phaser_data_t;
typedef struct { int equipped; unsigned char compatible[10]; } torpedo_data_t;
// This union combines all tactical modules into 1

typedef union ModuleData_t {
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
} moduledata_t;

typedef struct ModuleInfo_t {
    uint8_t state;  // online/offline
    uint8_t modclass, modtype, techid;
    uint8_t location;   // see ShipZones
    health_t health;
    power_t power;
    moduledata_t info;
} moduleinfo_t;
extern moduleinfo_t ModuleInfo;

#endif
