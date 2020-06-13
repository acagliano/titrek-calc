#ifndef equates_h
#define equates_h

#include <srldrvce.h>
#include <stdbool.h>
#include <graphx.h>
#include "classes/ships.h"

#define xStart 10
#define yStart 14
#define vWidth 300
#define vHeight 170


extern ship_t Ship;
extern gfx_rletsprite_t* err_icon;
extern srl_device_t srl;

typedef struct {
    bool exit;
    bool loopgame;
    bool network;
    bool logged_in;
} flags_t;
extern flags_t gameflags;

extern char *versionstr;
enum Versioning {
    STABLE,
    PREALPH,
    ALPHA,
    BETA
};

typedef struct {
    char username[24];
    char passwd[32];
    char email[64];
} userinfo_t;
extern userinfo_t userinfo;

#endif
