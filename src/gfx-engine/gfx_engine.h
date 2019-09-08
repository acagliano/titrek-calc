
#ifndef gfx_engine_h
#define gfx_engine_h
#include "classes/coords.h"

//Sort objects by Z axis. Assumes they've already been transformed.
void SortByDepth(mapentity_t *objects, unsigned int len);

//Translate len objects by vector
void tfm_translate(mapentity_t *objects, unsigned int len, vec3_t vector);

//Rotate len objects by vector
void tfm_rotate(mapentity_t *objects, unsigned int len, rad2_t vector);

//Scale len objects by vector
void tfm_scale(mapentity_t *objects, unsigned int len, vec3_t vector);

//Server-side? Dummy data output for now, soon.
void GatherObjects(mapentity_t *objects, vec3_t start, uint8_t rotmin, uint8_t rotmax);


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
    int x; uint8_t xr;
    int y; uint8_t yr;
    int z; uint8_t zr;
} vec3_t;
typedef vec3_t coords_t;

typedef struct {
    uint8_t xz,yz,xy;
} rad3_t;

typedef struct {
    unsigned char objectid;     // Cooresponds to enum Objects, and index of sprite
    vec3_t coords;            // access using [mapentity_t].coords.{x,y,z}
    unsigned int radius;        // Radius of round object, or max size of irregular
    bool irregular;             // True if object is irregular
    int rand_seed;              // Seed used to generate object (if irregular) - server-supplied
} mapentity_t;


#endif
