
#ifndef ship_h
#define ship_h

#define SHIP_MAX_MODULES 16

enum _system_types {
    SYS_EMPTY,
    SYS_POWER,
    SYS_MAIN,
    SYS_WEAPON,
    SYS_DEFENSE,
    SYS_MISC,
    SYS_SLOT_UNAVAILABLE = 0xff
};

typedef struct _health_t {
    uint8_t current;
    uint8_t max;
} health_t;

typedef struct _power_t {
    uint8_t current;
    uint8_t required;
} power_t;

typedef struct _ship_system_data {
    uint8_t system_type;
    uint8_t online_status;  // online/offline
    uint8_t system_name[11];
    health_t health;
    power_t power;
} ship_system_data_t;
extern ship_system_data_t ship_systems[SHIP_MAX_MODULES];


void ship_get_info(void);
void ship_render_interior(void);
void ship_render_sysoverview(ship_system_data_t* sys);
void ship_render_sysextended(void* sysext);

#endif
