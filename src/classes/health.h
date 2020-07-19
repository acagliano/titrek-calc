
#ifndef health_h
#define health_h

// Defines the Health class
// a sub-class of type moduleinfo_t
// Required by: modules.h

typedef struct {
    signed int max;
    signed int current;
} health_t;
// Related Functions
#define health_GetPercent(health)   (health)->current * 100 / (health)->max
#define health_SetHealth(health, amount)    (health)->current = amount

#endif
