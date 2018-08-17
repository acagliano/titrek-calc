#ifndef shipmodules_H
#define shipmodules_H

#include <stdbool.h>

// ENUMERATIONS OF MODULE TYPES, SUBTYPES
#define POWER_DEFAULT 5

enum modtype {
    // 0 is a NULL module
    mt_system = 1,
    mt_weapon,
    mt_shield,
    mt_tech,
    mt_ejectedcore
};

enum techtype {
    tt_shield,
    tt_integrity,
    tt_lifesupport,
    tt_warpcore,
    tt_warpdrive,
    tt_impulsedrive,
    tt_phaser,
    tt_torpedo,
    tt_transporter,
    tt_sensor,
    tt_auxiliary
};

enum locations {
    // 0 is not going to trigger alert
    saucer = 1,
    aft,
    nacelles
};
 // MODULE STATS STRUCTS

typedef struct {
    unsigned char equipped; // torpedoes only
    unsigned char charge;  // phasers only
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
    char modtype;      // system, weapon, shield, misc
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
    union stats {
        weapstats_t weapstats;
        sysstats_t sysstats;
        shieldstats_t shieldstats;
    } stats;
} Module_t;

#endif
