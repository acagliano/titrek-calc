#ifndef imgcaching_h
#define imgcachine_h

#include <stddef.h>
#include <stdint.h>
#include <graphx.h>

typedef struct _cache_entry {
    gfx_rletsprite_t* sprite;
    uint24_t age;
} cache_entry_t;

extern cache_entry_t spritecache[256];

/*
CACHE_INSERT()
    data = PTR to sprite data to insert
    slot = cache slot to write metadata to
    size = guesstimate of sprite size to copy
    RETURNS = false on failure (usually malloc error), true on success
 */
bool cache_insert(gfx_rletsprite_t* data, uint8_t slot, size_t size);

/*
CACHE_REMOVE()
    slot = cache slot to free and clear metadata
 */
void cache_remove(uint8_t slot);

/*
CACHE_PURGE()
    clear all cache entries
    this prevents RAM leaks from sprites still in cache when you disconnect
 */
 void cache_purge(void);





#endif
