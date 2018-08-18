#ifndef shipmodules_H
#define shipmodules_H

#include <stdbool.h>

// ENUMERATIONS OF MODULE TYPES, SUBTYPES
#define POWER_DEFAULT 5

enum techtype {
    tt_shield = 1,
    tt_integrity,
    tt_lifesupport,
    tt_warpcore,
    tt_warpdrive,
    tt_impulsedrive,
    tt_phaser,
    tt_torpedo,
    tt_transporter,
    tt_sensor,
    tt_auxiliary,
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
    char equipped; // torpedoes only
    char charge, maxCharge;  // phasers only
    char damage_shield, damage_hull;
    char range;
    char speed;
} weapstats_t;

typedef struct {
    char resistance;
    char powerOut;
    char navSensRange;
    char targSensRange;
    char topSpeed;
} sysstats_t;

typedef struct {
    char resistance;       // how much damage shield can absorb at 100% before bleedthrough
    char modulation;
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
