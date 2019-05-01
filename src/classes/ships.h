#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include "tech.h"
// POWER CONTROL STRUCTURE
#define DRAW_CORE 0
#define DRAW_AUX 1
#define DRAW_RESERVE 2

enum ModuleTypes {
    INTEG,
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
typedef struct { int maxspeed; } engine_data_t;
typedef struct { int maxrange; } navsens_data_t;
typedef struct { int maxrange; int maxtargets; } trans_data_t;

//Tactical Structs
typedef struct { int maxrange; char maxtargets; } targsens_data_t;
typedef struct { unsigned int resistance;  int frequency; int maxhealth; int shieldhealth[SHIP_ALL]; } shield_data_t;
typedef struct { int yield; int range; int speed; } phaser_data_t;
typedef struct { int equipped; unsigned char compatible[10]; } torpedo_data_t;
// This union combines all tactical modules into 1

// for miscellaneous, there is no types, just ids.

typedef struct {
    char priority;          // determines which module draws power first
    signed int capacity;        // how much power a module can store
    signed int current;         // the amount of power the module currently has
    signed int spend;           // power spend per cycle
    signed int base;           // default power usage
    bool alwaysUse;          // boolean to specify if the module is always using power when active
    char drawFrom;
} power_t;
// Related Functions
#define POWER_INC 1
#define POWER_DEC -1
#define POWSRC_WARP 0
#define POWSRC_AUX 1
#define POWSRC_RESERVE 2
signed int power_GetBatteryPercent(power_t* power); // returns % of battery remain
signed int power_GetSpendPercent(power_t* power);   // returns power expend/usage
signed int power_GetPowerSpend(power_t* power);
void power_ChangeSpend(power_t* power, char amount);
signed int power_GetPowerDraw(power_t* power);
//bool power_ExpendThisCycle(power_t* power);     // returns if module should use power
//bool power_ReserveThisCycle(power_t* power);      // returns if module should recieve power
void power_SetDrawSource(power_t* power, char source);
char power_GetDrawSource(power_t* power);
/*#define power_SetDrawCore(power_t* power) \
power_SetDraw(power_t* power, DRAW_CORE)    // set module to draw from warp core
#define power_SetDrawAux(power_t* power) \
power_SetDraw(power_t* power, DRAW_AUX)     // set module to draw from auxiliary power
#define power_SetDrawReserve(power_t* power) \
power_SetDraw(power_t* power, DRAW_RESERVE) // set module to draw from reserve power */


// HEALTH MONITORING STRUCTURE
typedef struct {
    signed int max;
    signed int current;
} health_t;
// Related Functions
signed int health_GetHealthPercent(health_t* health);
void health_DamageModule(health_t* health, int amount);


// Module Online States
#define STATE_OFFLINE 0
#define STATE_ONLINE 1
#define STATE_REPAIRING 2
// Module Set State Error Codes
#define SETSTATE_SUCCESS 0
#define SETSTATE_NOPOWER 1
#define SETSTATE_NOHEALTH 2
// Module Effectiveness Step Values
#define STEP_DEFAULT 20
#define STEP_MID 10
#define STEP_LOW 5

// Module Data Struct
typedef union Data_t {
    // Core System Modules
    trans_data_t transport;
    integ_data_t integ;
    lifesup_data_t lifesupport;
    core_data_t core;
    engine_data_t engine;
    navsens_data_t navsens;
    // Shield Modules
    shield_data_t shields;
    // Weapons Modules
    torpedo_data_t torpedoes;
    phaser_data_t phasers;
    targsens_data_t targsens;
} data_t;








typedef struct {
    bool unlocked, assigned;
    unsigned char techid;
    unsigned char techtype;     // locked, determines compatible tech type
//    unsigned char techclass;    // locked, determines compatible module classes
    bool online, typelocked;            // is module online
    power_t power;          // power control
    health_t health;        // health monitor
    data_t data;
} module_t;
char module_GetOnlineState(module_t* module);   // return online, offline, or repairing
char module_SetOnlineState(module_t* module, char state);   // set module state or return no power or no health
//int module_GetEffectiveness(module_t* module, char steps);  // get % effectiveness of module
    // steps indicate how quickly changes to health or power of a module effect performance.
    // more steps equal greater sensitivity, but less fluctuation in performance
    // less steps equal lower sensitivity, but more fluctuation in performance
    // function return value will be based on sensitivity.
// EX: 73% of STEP_NORMAL might return 73% of 20 (16), or 70% effectiveness.
// EX: 73% of STEP_LOW would return 73% of 5 (3), or 60% effectiveness.

#define TACT_MAX 6
typedef struct {
    coords_t coords;
    health_t hull;      // hull hitpoints
    module_t system[SYS_MAX];
    module_t tactical[TACT_MAX];
    unsigned char sys_selected, def_selected, off_selected;
} ship_t;
// Core system defines

void module_SetHealthMax(health_t* health, int max);
void module_SetPowerMax(power_t* power);

#endif
