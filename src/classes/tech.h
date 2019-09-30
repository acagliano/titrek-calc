
#ifndef tech_h
#define tech_h



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
    int protection;
} shield_stats_t;

typedef struct {
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

typedef struct {
    int powerdrain;
    union Module {
        weapon_stats_t weapon_stats;
        sensor_stats_t sensor_stats;
        integrity_stats_t integrity_stats;
        shield_stats_t shield_stats;
        generator_stats_t generator_stats;
        engine_stats_t engine_stats;
    } module;
} module_stats_t;



typedef struct {
    int techclass;      // cooresponds to TechClasses above
    int techtype;   // used to determine if compatible
    int techid;     // this is just its place in array, but also its id
    int powerdrain; // how much base power this module uses
    union techinfo {
        module_stats_t module_stats;
        weapon_stats_t weapon_stats;
    } techinfo;
} tech_stats_t;

#endif
