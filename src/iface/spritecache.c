#include <stdbool.h>
#include <graphx.h>
#include <fileioc.h>

uint8_t* gfx_cache_file;
const char gfx_fname = "_trekgfx";

bool sprite_open_cache(void){
	return (gfx_cache_file = ti_Open(gfx_fname, "w+"));
}

gfx_rletsprite_t *sprite_append_to_cache(uint8_t *data, size_t len){
	gfx_rletsprite_t *sprite_ptr = ti_GetDataPtr(gfx_cache_file);
	if(ti_Write(data, len, 1, gfx_cache_file)) return sprite_ptr;
	else return NULL;
}

bool sprite_close_cache(void){
	return ti_Close(gfx_cache_file);
}
