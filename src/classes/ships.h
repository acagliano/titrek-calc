#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include <stdint.h>
#include "tech.h"
#include "systems/engines.h"

// Module QuickRef Structure (Used to Populate GUI)

enum StatusFlags {
    ONLINE,
    LOW_POWER,
    LOW_HEALTH,
    CRIT_HEALTH,
    OVERCHARGE
};


typedef struct {
    uint8_t techclass, techtype;
    uint8_t health;   // percentages
    uint8_t status_flags;
} module_t;
#define module_OnlineState(module)   ((module->status_flags >> ONLINE) & 1)
#define module_PowerWarn(module)     ((module->status_flags >> LOW_POWER) & 1)
#define module_HeathWarn(module)        ((module->status_flags >> LOW_HEALTH) & 1)
#define module_HealthWarnCrit(module)   ((module->status_flags >> CRIT_HEALTH) & 1)
#define module_Overcharge(module)       ((module->status_flags >> OVERCHARGE) & 1)

typedef struct {
    uint8_t health;
    //uint8_t composite[5];
    // other data here?
} hull_t;

typedef struct {
    uint8_t yaw;
    uint8_t pitch;
    uint8_t roll;
    uint24_t speed;
} nav_t;


#define MAX_MODULES 15
// we may need to change this if there's no room on screen for 9 tactical modules
typedef struct {
    uint24_t crew;
    hull_t hull;
    module_t system[MAX_MODULES];
    nav_t nav;
} ship_t;
extern ship_t Ship;
// Core system defines

// Here will be some defines used for some other things.
extern engine_data_t engine_ref[3];

#endif
