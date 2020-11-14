#ifndef equates_h
#define equates_h

#include <srldrvce.h>
#include <stdbool.h>
#include <graphx.h>
#include "classes/ships.h"
#include "classes/modules.h"
#include "rendering/screens.h"

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
    bool gfx_error;
    bool gfx_custom;
} flags_t;
extern flags_t gameflags;

typedef struct {
    bool network_up;
    bool bridge_up;
    bool bridge_error;
    bool client_version_ok;
    bool logged_in;
    bool game_loaded;
} net_flags_t;
extern net_flags_t netflags;

extern selected_t select;
extern uint16_t screen;

extern uint8_t version[3];
extern char versionstr[12];
extern uint8_t gfx_version[2];
extern uint8_t gfx_reqd[2];
extern uint8_t gfx_custom[2];
extern uint24_t ntwk_inactive_clock;
extern moduleinfo_t ModuleInfo;

extern gfx_rletsprite_t* icon_netup;
extern gfx_rletsprite_t* icon_netdown;
extern gfx_rletsprite_t* log_error;
extern gfx_rletsprite_t* log_info;
extern gfx_rletsprite_t* log_debug;
extern gfx_rletsprite_t* log_server;

#endif
