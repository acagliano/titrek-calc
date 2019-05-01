
#ifndef gfx_engine_h
#define gfx_engine_h
#include "classes/coords.h"

// ENGINE TYPEDEFS
enum Objects {
    STAR,
    PLANET,
    STARBASE,
    ASTEROID,
    COMET,
    BLACKHOLE,
    SHIP,
    TORPEDO,
    PHASER,
    EJ_CORE,
};

enum Particles {
    DMG_INT,    // Sparks that fly when internal damage occurs (renders to player)
    DMG_EXT,    // Smoke trail that renders in space when ship damage (renders to other players)
    EXPLODE     // Explosions (any kind)
}

typedef struct {
    unsigned char objectid;     // Cooresponds to enum Objects, and index of sprite
    coords_t coords;            // access using [mapentity_t].coords.{x,y,z}
    unsigned int radius;        // Radius of round object, or max size of irregular
    bool irregular;             // True if object is irregular
    int rand_seed;              // Seed used to generate object (if irregular) - server-supplied
} mapentity_t;


#endif
