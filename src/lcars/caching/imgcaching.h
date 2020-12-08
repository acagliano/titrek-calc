#ifndef imgcaching_h
#define imgcaching_h

#include <stddef.h>
#include <stdint.h>
#include <graphx.h>


extern gfx_sprite_t* terraincache[100];
extern gfx_sprite_t* playercache[100];

enum _sprite_type {
    SPRITE_TERRAIN,
    SPRITE_PLAYER
};

/*
CACHE_INSERT()
    data = PTR to sprite data to insert
    slot = cache slot to write metadata to
    size = guesstimate of sprite size to copy
    RETURNS = false on failure (usually malloc error), true on success
 */
bool cache_insert(gfx_rletsprite_t* data, uint8_t type, uint8_t slot, uint24_t size);

/*
CACHE_REMOVE()
    slot = cache slot to free and clear metadata
 */
void cache_remove(uint8_t type, uint8_t slot);

/*
CACHE_PURGE()
    clear all cache entries
    this prevents RAM leaks from sprites still in cache when you disconnect
 */
 void cache_purge(void);





#endif
