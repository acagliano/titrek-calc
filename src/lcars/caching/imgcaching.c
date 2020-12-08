
#include <stdio.h>
#include <graphx.h>
#include <compression.h>
#include <string.h>
#include "imgcaching.h"
#include "../../equates.h"

/* SPRITE CACHIMG SYSTEM */
/*
    This "library" of functions implements a 256-entry sprite cache

*/

gfx_sprite_t* terraincache[100] = {NULL};
gfx_sprite_t* playercache[100] = {NULL};



bool cache_insert(gfx_rletsprite_t* data, uint8_t type, uint8_t slot, uint24_t size){
// This will be the function used to insert a sprite into a given cache slot
// Using the client ID to which the sprite belongs
    gfx_sprite_t* dest;
    if(!(dest=malloc(size)))
        return false;
    zx7_Decompress(dest, data);
    if(type==SPRITE_TERRAIN) terraincache[slot] = dest;
    if(type==SPRITE_PLAYER) playercache[slot] = dest;
    return true;
}

void cache_remove(uint8_t type, uint8_t slot){
    if(type==SPRITE_TERRAIN) {free(terraincache[slot]); terraincache[slot] = NULL;}
    if(type==SPRITE_PLAYER) {free(playercache[slot]); playercache[slot] = NULL;}
}

void cache_purge(void){
    uint24_t i = 0;
    do {
        cache_remove(SPRITE_TERRAIN, i);
        cache_remove(SPRITE_PLAYER, i);
    } while( i++ <= 255 );
}
