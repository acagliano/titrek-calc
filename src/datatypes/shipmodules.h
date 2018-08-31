#ifndef shipmodules_H
#define shipmodules_H

#include <stdbool.h>

// ENUMERATIONS OF MODULE TYPES, SUBTYPES
#define POWER_DEFAULT 5

enum techtype {
    tt_null = 0,
    tt_shield = 1,
    tt_integrity,
    tt_lifesupport,
    tt_warpdrive,
    tt_impulsedrive,
    tt_phaser,
    tt_torpedo,
    tt_sensor,
    tt_transporter = 9,
    tt_warpcore = 14,
    tt_auxiliary = 15,
    tt_ejectedcore
};

enum locations {
    // 0 is not going to trigger alert
    saucer = 1,
    aft,
    nacelles
};
 // MODULE STATS STRUCTS

typedef struct {
    char weapname[11];
    bool weaponready;
    char equipped; // torpedoes only
    char cooldown;
    char charge, maxCharge;  // phasers only
    char damage_shield, damage_hull;
    char range;
    char speed;
} weapstats_t;

typedef struct {
    char resistance;
    char powerOut;
    char powerOutActual;
    int sensor_range;
    int targSensRange;
    char topSpeed;
    int auxiliarymax;
} sysstats_t;

typedef struct {
    char resistance;       // how much damage shield can absorb at 100% before bleedthrough
    char modulation;
    short healthForward;
    short healthAft;
    short healthStarboard;
    short healthPort;
} shieldstats_t;


// MODULE DEFAULT STRUCT DECLARATION

typedef struct {
    char techtype;  // module class (for verifiying proper upgrade type)
    char techid;   // for retrieving specific module data
    char techname[11];
    bool online;      // used for everything  is module online
    bool upgradeable;   // flag to specify if a module may be upgraded
    char powerDraw;      // used for everything  module power consumption
    char powerDefault;
    bool pdConstant;
    int powerReserve;      // used for everything  module power reserve
    short health, maxHealth;
    char location;
    union {
        weapstats_t weapstats;
        sysstats_t sysstats;
        shieldstats_t shieldstats;
    } stats;
} Module_t;

#endif
