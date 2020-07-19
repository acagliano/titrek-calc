#ifndef power_h
#define power_h

// Defines the Power class
// a sub-class of type moduleinfo_t
// Required by: modules.h

#define SRC_WARP 0
#define SRC_AUX 1
#define SRC_RESERVE 2

typedef struct {
    char priority;          // determines which module draws power first
    signed int capacity;        // how much power a module can store
    signed int current;         // the amount of power the module currently has
    signed int spend;           // power spend per cycle
    signed int base;           // default power usage
    bool alwaysUse;          // boolean to specify if the module is always using power when active
    char source;
} power_t;

#define power_GetReservePercent(power) ((power)->current * 100 / (power)->capacity)
#define power_GetUsagePercent(power) ((power)->spend * 100 / (power)->base)
#define power_SetSpend(power, amount)   (power)->spend = amount
#define power_SetSource(power, source)  (power)->source = source  // use defines above

#endif
