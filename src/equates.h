#ifndef equates_h
#define equates_h

#include <srldrvce.h>
#include <stdbool.h>
#include <graphx.h>

#define xStart 10
#define yStart 14
#define vWidth 300
#define vHeight 170

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

#endif
