#ifndef equates_h
#define equates_h

#include <srldrvce.h>
#include <stdbool.h>
#include <graphx.h>
#include "classes/ships.h"
#include "classes/modules.h"

#define xStart 10
#define yStart 14
#define vWidth 300
#define vHeight 170
#define MODICONS 14     // define first module icon



extern ship_t Ship;
extern gfx_rletsprite_t* err_icon;

typedef struct {
    bool exit;
    bool loopgame;
    bool network;
    bool logged_in;
    bool ship_loaded;
    bool gfx_error;
    bool gfx_custom;
} flags_t;
extern flags_t gameflags;

extern uint8_t version[3];
extern char versionstr[12];
extern uint8_t gfx_version[2];
extern uint8_t gfx_reqd[2];
extern uint8_t gfx_custom[2];
extern uint24_t ntwk_inactive_clock;
extern moduleinfo_t ModuleInfo;

extern gfx_rletsprite_t* icon_netup;
extern gfx_rletsprite_t* icon_netdown;

#endif
