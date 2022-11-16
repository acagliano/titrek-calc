#include <stdbool.h>
#include <graphx.h>
#include <fileioc.h>
#include "spritecache.h"

uint8_t* gfx_cache_file;
const char gfx_fname = "_trekgfx";

size_t gfx_cache_ct = 0;
gfx_rletsprite_t *gfx_cache_ptrs[GFX_CACHE_MAX] = {NULL};


bool sprite_open_cache(void){
	return (gfx_cache_file = ti_Open(gfx_fname, "w+"));
}

gfx_rletsprite_t *sprite_append_to_cache(uint8_t *data, size_t len){
	gfx_rletsprite_t *sprite_ptr = ti_GetDataPtr(gfx_cache_file);
	if(gfx_cache_ct==GFX_CACHE_MAX) return NULL;
	if(ti_Write(data, len, 1, gfx_cache_file)) {
		gfx_cache_ct++;
		return sprite_ptr;
	}
	else return NULL;
}

bool sprite_close_cache(void){
	return ti_Close(gfx_cache_file);
}
