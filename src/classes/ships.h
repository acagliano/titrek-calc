#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include <stdint.h>
#include "tech.h"

// Module QuickRef Structure (Used to Populate GUI)

typedef struct {
    uint8_t techclass, techtype, techid;
    bool online;
    uint8_t health, power;   // percentages
} module_t;
#define module_GetOnlineState(module)   module->online
#define module_SetOnline(module)    module->online = STATE_ONLINE
#define module_SetOffline(module)   module->online = STATE_OFFLINE
#define module_SetRepairing(module) module->online = STATE_REPAIR

typedef struct {
    uint8_t health;
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
