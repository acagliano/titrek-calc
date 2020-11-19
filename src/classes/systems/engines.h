#ifndef engines_h
#define engines_h

#include <stdbool.h>

typedef struct {
    uint24_t max_speed;
    uint24_t max_accel;
    uint24_t current_speed;
} engine_data_t;

typedef struct {
    bool loaded;
    engine_data_t engine[3];
} engine_ref_t;

#define engine_GetMaxSpeed(module)  (module->max_speed)
#define engine_GetMaxAccel(module)  (module->max_accel)
#define engine_GetCurrentSpeed(module) (module->current_speed)

#endif
