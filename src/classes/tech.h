
#ifndef tech_h
#define tech_h

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

typedef union {
    weapon_stats_t weapon_stats;
    sensor_stats_t sensor_stats;
    integrity_stats_t integrity_stats;
    shield_stats_t shield_stats;
    generator_stats_t generator_stats;
    engine_stats_t engine_stats;
} stats_t;


#endif
