#ifndef render_h
#define render_h
#include <graphx.h>
#include "datatypes/mapdata.h"
#include "datatypes/playerdata.h"

typedef struct {
    gfx_sprite_t* uncompressed;
    gfx_sprite_t* rotated;
    gfx_sprite_t* scaled;
} buffers_t;



char GUI_PrepareFrame(MapData_t *map, renderitem_t *renderbuffer, Position_t *playerpos);
void GUI_RenderFrame(gfx_sprite_t **sprites, buffers_t *buffers, renderitem_t *renderbuffer, char count);
void gfxinit_DecompressAll(gfx_sprite_t **array);
void gfxinit_FreeAll(gfx_sprite_t **array);
int compare_objects(const void *p, const void *q);
//void GUI_RenderFrame(framedata_t *frame, renderitem_t *renderbuffer);


#endif
