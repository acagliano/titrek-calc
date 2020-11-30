
#include <stdio.h>
#include <string.h>
#include "imgcaching.h"
#include "../../equates.h"

/* SPRITE CACHIMG SYSTEM */
/*
    This "library" of functions implements a 256-entry sprite cache

*/

cache_entry_t spritecache[256] = {0};



bool cache_insert(gfx_rletsprite_t* data, uint8_t slot, size_t size){
// This will be the function used to insert a sprite into a given cache slot
// Using the client ID to which the sprite belongs
    gfx_rletsprite_t* dest = malloc(size);
    cache_entry_t* cache = &spritecache[slot];
    if(!dest) return false;
    memcpy(dest, data, size);
    cache->sprite = dest;
    return true;
}

void cache_remove(uint8_t slot){
    cache_entry_t* cache = &spritecache[slot];
    if(cache->sprite){
        free(cache->sprite);
        cache->sprite = NULL;
    }
}

void cache_purge(void){
    uint24_t i = 0;
    do {
        cache_remove(i);
    } while( i++ < 255 );
}
