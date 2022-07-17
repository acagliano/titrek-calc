
#ifndef ship_h
#define ship_h

#define SHIP_MAX_MODULES 16

enum _system_types {
    SYS_EMPTY,
    SYS_CORE,
    SYS_WEAPON,
    SYS_DEFENSE,
    SYS_MISC,
    SYS_SLOT_UNAVAILABLE = 0xff
};

typedef struct _ship_system_data {
    uint8_t system_type;
    uint8_t online_status;  // online/offline
    uint8_t system_name[11];
    uint8_t system_health_percent;
    uint8_t system_power_percent;
} ship_system_data_t;
extern ship_system_data_t ship_systems[SHIP_MAX_MODULES];


void ship_load_info(void);
void ship_render_interior(void);

#endif
