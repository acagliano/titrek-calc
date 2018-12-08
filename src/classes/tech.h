
#ifndef tech_h
#define tech_h

enum TechClasses {
    TECHCLASS_SHIPMODULE,
    TECHCLASS_PROJECTILE
};

enum TechTypes {
    TECHTYPE_SHIELD = 1,
    TECHTYPE_INTEG,
    TECHTYPE_WARPCORE,
    TECHTYPE_WARPDRIVE,
    TECHTYPE_IMPULSE,
    TECHTYPE_LIFESUPPORT,
    TECHTYPE_SENSORS,
    TECHTYPE_TRANSPORT,
    TECHTYPE_PHASER,
    TECHTYPE_DISRUPTOR,
    TECHTYPE_TORPEDO
};

enum ProjectileTypes {
    PROJECTILE_SPATIAL = 1,
    PROJECTILE_PHOTON,
    PROJECTILE_QUANTUM,
    PROJECTILE_ANTIMATTER,
    PROJECTILE_CHRONITON,
    PROJECTILE_TRANSPHASIC,
    PROJECTILE_NEROMAIN,
    PROJECTILE_NEROSPAWN
};

typedef struct {
    int powerdrain;
    int protection;
} shield_stats_t;

typedef struct {
    int powerdrain;
    int protection;
    int weight;
} integrity_stats_t;

typedef struct {
    int powerout;
} generator_stats_t;

typedef struct {
    int maxspeed;
} engine_stats_t;

typedef struct {
    int powerdrain;
    int targetxz;       // angles for targetting
    int targetxy;       // angles for targetting
    int maxrange;
} sensor_stats_t;

typedef struct {
    int firingspeed;
    int reloadtime;
    int shielddamage;
    int normaldamage;
} weapon_stats_t;

typedef union {
    weapon_stats_t weapon_stats;
    sensor_stats_t sensor_stats;
    integrity_stats_t integrity_stats;
    shield_stats_t shield_stats;
    generator_stats_t generator_stats;
    engine_stats_t engine_stats;
} module_stats_t;

typedef union {
    module_stats_t module_stats;
    projectile_stats_t projectile_stats;
} sub_stats_t;


typedef struct {
    int techid;     // this is just its place in array, but also its id
    int techclass;   // ship module, projectile, etc
    int techtype;   // used to determine if compatible
    int powerdrain; // how much base power this module uses
    sub_stats_t techinfo;    // import tech-specific info
} stats_t;

#endif
