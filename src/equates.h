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


extern ship_t Ship;
extern gfx_rletsprite_t* err_icon;

typedef struct {
    bool exit;
    bool loopgame;
    bool network;
    bool logged_in;
} flags_t;
extern flags_t gameflags;

extern char version[3];
extern char versionstr[12];
extern moduleinfo_t ModuleInfo;

#endif
