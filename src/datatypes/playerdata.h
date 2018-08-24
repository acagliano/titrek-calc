
#ifndef playerdata_H
#define playerdata_H
#include <stdbool.h>

enum Achievements {
    achieve_NoDamage,   // defeat an enemy ship without taking structure damage (shields dont count)
    achieve_NoShot,     // defeat an enemy without firing a shot
    achieve_TurningtheTide, // come back from almost disabled to win battle
};

enum InventoryItemTypes {
    // 0 = null
    it_module = 1,
    it_torpedo,
    it_titanium,
    it_deuterium
};

enum timers {
    timer_corebreach,
    timer_lifesupport,
    timer_torpcooldown
};

#define KEY_INTERVAL 1
#define POWER_INTERVAL 5
#define CORE_RECHARGE_TIMER 20
#define CORE_BREACH_TIMER 1000
#define LIFE_SUPPORT_TIMER 2000
#define REPAIR_INTERVAL 10

#define COORD_X 0
#define COORD_Y 1
#define COORD_Z 2
#define angle_xy 0
#define angle_z 1
#define MODE_PHASER 0
#define MODE_PHOTON 1

typedef struct {
    char speed;
    int coords[3];
    short vectors[2];
    unsigned char angles[2];
} Position_t;

typedef struct {
    unsigned char angles[2];
    char target;
} target_t;

typedef struct {
    char itemtype;
    char itemtechtype;  // only for modules or torpedoes
    char itemdamage; // only for modules
    char itemquantity;
    // char itemquantitymax;
} Inventory_t;

typedef struct {
    Position_t position;
    Inventory_t inventory[20];
    target_t target;
    unsigned long tick;
    short timers[3];
    int money;
    char modulation;
    char Achievements[8];
    char iconStates[2];
    bool damagesection[3];
    char ScreenSelected;  // default, status, power, tactical, chat, sensors
    char ScreenSecondary;
    char moduleSelected;
    char moduleRepairing;
    char deathreason;
    char weaponMode;
    char refreshRate;
} Player_t;

enum ScreenSelections {
    SCRN_START,
    SCRN_VIEW,
    SCRN_TACTICAL,
    SCRN_STATUS,
    SCRN_POWER,
    SCRN_MAP,
    SCRN_INVENTORY,
    SCRN_SENSORS
};
enum ScreenSecondary {
    DEFAULT = 0,
    SCRN_VIEW_NAV,
    SCRN_VIEW_TACT
};

#define REFRESH_RATE 20
#define ALERT_REFRESH 80


#endif
