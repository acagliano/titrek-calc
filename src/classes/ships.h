#ifndef ships_h
#define ships_h

#include <stdbool.h>
#include "tech.h"
// POWER CONTROL STRUCTURE
#define DRAW_CORE 0
#define DRAW_AUX 1
#define DRAW_RESERVE 2

enum ModuleClasses {
    MODCLASS_SYSTEM = 1,
    MODCLASS_TACTICAL,
    MODCLASS_MISC
};

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
typedef struct {
    unsigned char techclass;    // locked, determines compatible module classes
    unsigned char techtype;     // locked, determines compatible modules
    unsigned char techid;   // corresponds to equipped tech id (tech.h)
    char online;            // is module online
    power_t power;          // power control
    health_t health;        // health monitor
    stats_t stats;
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


typedef struct {
    module_t integrity;
    module_t warpcore;
    module_t warpdrive;
    module_t impulsedrive;
    module_t lifesupport;
    module_t sensors;
    module_t transporters;
    module_t tactical[6];    // tactical or shield modules
    module_t misc[3];       // miscellaneous modules
} ship_t;
// Core system defines
#define integrity (module_t*)&ship->integrity;
#define warpcore (module_t*)&ship->warpcore;
#define warpdrive (module_t*)&ship->warpdrive;
#define impulsedrive (module_t*)&ship->impulsedrive;
#define lifesupport (module_t*)&ship->lifesupport;
#define sensors (module_t*)&ship->sensors;
#define transporters (module_t*)&ship->transporters;

#endif
