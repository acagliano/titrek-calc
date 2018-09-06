#ifndef mapdata_h
#define mapdata_h
#include "playerdata.h"
#include <stdbool.h>

#define RENDER_DISTANCE 50
#define FOV 45

enum EntityTypes {
    // 0 = NULL
    et_ship = 1,
    et_phaser,
    et_photon_projectile,
    et_quantum_projectile,
    /*et_star,
    et_planet,
    et_asteroid,
    et_blackhole,
    et_supernova,
    et_ejectedcore,*/
    shield_flare,
    explosion
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
} entity_misc_t;        // structure type for ejected core and projectile

typedef struct {
    char weaponid;      // to identify weapon in database
    char shield_damage;
    char hull_damage;
    int range;
    char charge; // phasers
} entity_weap_t;


typedef struct {
    char entitytype;
    bool mobile;
    Position_t position;
    char speed;
    union {
        entity_ship_t ship;
        entity_default_t terrain;
        entity_weap_t weapon;
    } entitystats;
} MapData_t;

typedef struct {
    unsigned char spriteid;
    unsigned int x,y;
    unsigned int distance;
    char angle;
} renderitem_t;

#endif
