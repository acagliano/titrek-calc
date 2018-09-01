#ifndef render_h
#define render_h

#include "datatypes/mapdata.h"
#include "datatypes/playerdata.h"
typedef struct { char count; int pointer; } framedata_t;

void GUI_PrepareFrame(MapData_t *map, renderitem_t *renderbuffer, Position_t *playerpos, framedata_t* frame);
void GUI_RenderFrame(framedata_t *frame, renderitem_t *renderbuffer);


#endif
