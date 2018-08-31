#ifndef mapdata_h
#define mapdata_h
#include "playerdata.h"
#include <stdbool.h>

#define RENDER_DISTANCE 2500
#define FOV 45

enum EntityTypes {
    // 0 = NULL
    et_ship = 1,
    et_star,
    et_planet,
    et_asteroid,
    et_blackhole,
    et_supernova,
    et_ejectedcore,
    et_phaser_projectile,
    et_torpedo_projectile
};

typedef struct {
    int health, maxHealth;
    int shield, shieldMax;
} entity_ship_t;

typedef struct {
    char size;
    char gravity;
    char damage;
    char damageradius;
} entity_default_t;     // the structure type for star - supernova

typedef struct {
    char timer_remain; // for ejected core
    char weaponid;      // to identify weapon in database
} entity_misc_t;        // structure type for ejected core and projectile


typedef struct {
    char entitytype;
    bool mobile;
    Position_t position;
    union entitystats {
        entity_ship_t e_ship;
        entity_default_t e_default;
        entity_misc_t e_misc;
    } entitystats;
} MapData_t;

typedef struct {
    unsigned char spriteid;
    unsigned int x,y;
    unsigned int distance;
} renderitem_t;

#endif
