
#ifndef spritecache_h
#define spritecache_h

#include <stdbool.h>
#include <graphx.h>
#include <fileioc.h>

#define GFX_CACHE_MAX	128
extern size_t gfx_cache_ct;
extern gfx_rletsprite_t *gfx_cache_ptrs[GFX_CACHE_MAX];

bool sprite_open_cache(void);
bool sprite_close_cache(void);
gfx_rletsprite_t *sprite_append_to_cache(uint8_t *data, size_t len);


#endif
