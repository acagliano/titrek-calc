#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include <stdint.h>
#include "modules.h"
// POWER CONTROL STRUCTURE

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
    TARG_SENS
};

enum TechClasses {
    mUnassigned = 0,
    mSystem,
    mTactical
};


enum ShipZones {
    FORWARD,
    STARBOARD,
    AFT,
    PORT,
    DORSAL,
    VENTRAL,
    SHIP_ALL
};
// for the moment, these locations are unused, but will be used later to determine what systems to damage based on where they are located



// for miscellaneous, there is no types, just ids.
#define SOURCE_CORE 0   // recieve power from warp core
#define SOURCE_AUX 1    // recieve power from aux generator (requires this module installed)
#define SOURCE_RESERVE 2    // recieve power from internal reserve (battery)

typedef struct {
    char priority;          // determines which module draws power first
    signed int capacity;        // how much power a module can store
    signed int current;         // the amount of power the module currently has
    signed int spend;           // power spend per cycle
    signed int base;           // default power usage
    bool alwaysUse;          // boolean to specify if the module is always using power when active
    char source;
} power_t;
// Related Functions
#define POWER_INC 1
#define POWER_DEC -1
#define POWSRC_WARP 0
#define POWSRC_AUX 1
#define POWSRC_RESERVE 2
#define power_GetReservePercent(power) (power->current * 100 / power->capacity)
#define power_GetUsagePercent(power) (power->spend * 100 / power->base)
#define power_SetSpend(power, amount)   power->spend = amount
#define power_SetSource(power, source)  power->source = source  // use defines above

// HEALTH MONITORING STRUCTURE
typedef struct {
    signed int max;
    signed int current;
} health_t;
// Related Functions
#define health_GetPercent(health)   health->current * 100 / health->max
#define health_SetHealth(health, amount)    health->current = amount


// Module Online States
#define STATE_OFFLINE 0
#define STATE_ONLINE 1
#define STATE_REPAIR 2
// Module Set State Error Codes
#define SETSTATE_SUCCESS 0
#define SETSTATE_NOPOWER 1
#define SETSTATE_NOHEALTH 2

// Module Data Struct

typedef struct {
    bool unlocked,online,typelocked;
    uint8_t techid;
    uint8_t modclass;
    uint8_t techtype;     // locked, determines compatible tech type
//    unsigned char techclass;    // locked, determines compatible module classes
    power_t power;          // power control
    health_t health;        // health monitor
    uint8_t location;       // see ShipZones above
} module_t;
#define module_GetOnlineState(module)   module->online
#define module_SetOnline(module)    module->online = STATE_ONLINE
#define module_SetOffline(module)   module->online = STATE_OFFLINE
#define module_SetRepairing(module) module->online = STATE_REPAIR

typedef struct {
    health_t health;
    uint8_t composite[5];
    // other data here?
} hull_t;

typedef struct {
    uint8_t yaw;
    uint8_t pitch;
    uint8_t roll;
} rotations_t;


#define MAX_MODULES 15
// we may need to change this if there's no room on screen for 9 tactical modules
typedef struct {
    uint24_t crew;
    rotations_t rotate;
    hull_t hull;
    module_t system[MAX_MODULES];
} ship_t;
// Core system defines

#endif
