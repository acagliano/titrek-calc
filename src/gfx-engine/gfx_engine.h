
#ifndef gfx_engine_h
#define gfx_engine_h
//#include "classes/coords.h"

//Sort objects by Z axis. Assumes they've already been transformed.
unsigned int SortByDepth(mapentity_t **objects, unsigned int len);

//Translate len objects by vector
void tfm_translate(mapentity_t **objects, unsigned int len, vec3_t vector);

//Rotate len objects using scv, returned from setupSinCos.
void tfm_rotate(mapentity_t **objects, unsigned int len, uint8_t *scv);

//Scale len objects by vector
void tfm_scale(mapentity_t **objects, unsigned int len, vec3_t vector);

//Return a byte for each cos/sin in each xz/yz/xy rotation. In that order.
uint8_t *setupSinCos(rad3_t vector);

//Server-side? Dummy data output for now, soon.
void GatherObjects(mapentity_t **objects, vec3_t start, uint8_t rotmin, uint8_t rotmax);


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
    long x,y,z;
} vec3_t;
typedef vec3_t coords_t;

typedef struct {
    uint8_t xz,yz,xy;
} rad3_t;

typedef struct {
    unsigned char objectid;     // Cooresponds to enum Objects, and index of sprite
    vec3_t coords;              // access using [mapentity_t].coords.{x,y,z}
    unsigned int radius;        // Radius of round object, or max size of irregular
    int rand_seed;              // Seed used to generate object (if irregular) - server-supplied
    bool doZSort;               // The sorting methods use this to tell if this needs to be sorted.
} mapentity_t;


#endif
