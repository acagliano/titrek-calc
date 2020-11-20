#ifndef equates_h
#define equates_h

#include <stdbool.h>
#include <graphx.h>
#include "classes/ships.h"
#include "classes/modules.h"
#include "graphics/screens.h"

#define MODICONS 14     // define first module icon



extern gfx_rletsprite_t* err_icon;



extern uint24_t ntwk_inactive_clock;

extern gfx_rletsprite_t* icon_netup;
extern gfx_rletsprite_t* icon_netdown;
extern gfx_rletsprite_t* log_error;
extern gfx_rletsprite_t* log_info;
extern gfx_rletsprite_t* log_debug;
extern gfx_rletsprite_t* log_server;

#endif
