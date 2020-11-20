#include <stdint.h>
#include <graphx.h>
#include "engine.h"

#define SCRN_CENTER_X 160
#define SCRN_CENTER_Y 82
#define SCREEN_X 21
#define SCREEN_Y 15
#define SCREEN_XW (21 + 245)
#define SCREEN_YH (15 + 134)
// Screen Size (256, 134)
// X Offset 21
// Y Offset 15

void object_DrawBody(int8_t x, int8_t y, uint8_t color, uint8_t radius){
    gfx_SetColor(color);
    gfx_FillCircle(SCRN_CENTER_X + x, SCRN_CENTER_Y + y, radius);
}

void object_DrawAtmosphere(int8_t x, int8_t y, uint8_t color, uint8_t radius){
    gfx_SetColor(color);
    gfx_Circle(SCRN_CENTER_X + x, SCRN_CENTER_Y + y, radius);
}

void renderFrame(body_packet_t* frame){
    int24_t i;
    gfx_SetClipRegion(SCREEN_X, SCREEN_Y, SCREEN_XW, SCREEN_YH);
    for(i = 0; i < frame->numbodies; i++){
        frame_body_t *object = &frame->bodies[i];
        int8_t obj_x = object->x, obj_y = object->y;
        uint8_t obj_radius = object->radius;
        object_DrawBody(obj_x, obj_y, object->bodycolor, obj_radius);
        object_DrawAtmosphere(obj_x, obj_y, object->atmospherecolor, obj_radius);
    }
    gfx_ResetClip();
}
