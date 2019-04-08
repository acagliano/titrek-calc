#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include "tech.h"
// POWER CONTROL STRUCTURE
#define DRAW_CORE 0
#define DRAW_AUX 1
#define DRAW_RESERVE 2

enum ModuleClasses {
    MC_SYSTEM = 1,
    MC_TACTICAL,
    MC_MISC
};

enum SysTypes {
    M_INTEG,
    M_LIFESUP,
    M_CORE,
    M_WARPDR,
    M_IMPDR,
    M_NAVSENS,
    M_TRANS,
    M_SYSMAX
};

enum TactTypes {
    M_TACTSENS,
    M_SHIELD,
    M_PHASER,
    M_TORPEDO
};

//System Structs
typedef struct { int drv; } integ_data_t;
typedef struct { int occupancy; } lifesup_data_t;
typedef struct { int output; } core_data_t;
typedef struct { int maxspeed; } engine_data_t;
typedef struct { int maxrange; } navsens_data_t;
typedef struct { int maxrange; int maxtargets; } trans_data_t;

//Tactical Structs
typedef struct { int maxrange; int maxtargets; } targsens_data_t;
typedef struct { char placeholder; } shield_data_t;
typedef struct { int range; int speed; int shielddamage; int hulldamage; } weapon_data_t;
// This union combines all tactical modules into 1

typedef union System_t {
    trans_data_t transport;
    integ_data_t integrity;
    lifesup_data_t lifesupport;
    core_data_t core;
    engine_data_t engine;
    navsens_data_t navsens;
} system_t;

typedef union Tactical_t {
    weapon_data_t weapons;
    targsens_data_t targsens;
    shield_data_t shields;
} tactical_t;

typedef union Type_t {
    system_t system;
    tactical_t tactical;
} type_t;

// for miscellaneous, there is no types, just ids.

typedef struct {
    char priority;          // determines which module draws power first
    signed int capacity;        // how much power a module can store
    signed int current;         // the amount of power the module currently has
    signed int baseusage;           // baseline power usage
    signed int usage;          // how much power the module expends when active
    bool alwaysUse;          // boolean to specify if the module is always using power when active
    char drawFrom;
} power_t;
// Related Functions
signed int power_GetBatteryPercent(power_t* power); // returns % of battery remain
signed int power_GetExpendPercent(power_t* power);   // returns power expend/usage
//bool power_ExpendThisCycle(power_t* power);     // returns if module should use power
//bool power_ReserveThisCycle(power_t* power);      // returns if module should recieve power
void power_SetDraw(power_t* power, char source);
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
typedef struct {
    unsigned char techtype;
    char techname[10];
    type_t type;
} data_t;








typedef struct {
    unsigned char techid;
    unsigned char techclass;    // locked, determines compatible module classes
    char online;            // is module online
    power_t power;          // power control
    health_t health;        // health monitor
    data_t data;
} module_t;
char module_GetOnlineState(module_t* module);   // return online, offline, or repairing
char module_SetOnlineState(module_t* module, char state);   // set module state or return no power or no health
void* module_GetDataPtr(module_t* module);
void* module_GetSysDataPtr(data_t* data);
void* module_GetTactDataPtr(data_t* data);
//int module_GetEffectiveness(module_t* module, char steps);  // get % effectiveness of module
    // steps indicate how quickly changes to health or power of a module effect performance.
    // more steps equal greater sensitivity, but less fluctuation in performance
    // less steps equal lower sensitivity, but more fluctuation in performance
    // function return value will be based on sensitivity.
// EX: 73% of STEP_NORMAL might return 73% of 20 (16), or 70% effectiveness.
// EX: 73% of STEP_LOW would return 73% of 5 (3), or 60% effectiveness.

#define TACT_MAX 6

typedef struct {
    module_t system[M_SYSMAX];
    module_t tactical[TACT_MAX];    // tactical or shield modules
} ship_t;
// Core system defines
#define integrity (module_t*)&ship->system[M_INTEG];
#define lifesupport (module_t*)&ship->system[M_LIFESUP];
#define warpcore (module_t*)&ship->system[M_CORE];
#define warpdrive (module_t*)&ship->system[M_WARPDR];
#define impulsedrive (module_t*)&ship->system[M_IMPDR];
#define navsens (module_t*)&ship->system[M_NAVSENS];
#define transporters (module_t*)&ship->system[M_TRANS];
#define comms (module_t*)&ship->system[M_COMMS];
void module_SetHealthMax(health_t* health);
void module_SetPowerMax(power_t* power);

#endif
