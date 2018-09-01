#include "datatypes/mapdata.h"
#include "datatypes/playerdata.h"
#include "render.h"
#include <math.h>
#include <graphx.h>
#include <string.h>
#include <tice.h>
#include <mymath.h>
#include "equates.h"
#include "gfx/trekvfx.h"
#include <compression.h>


void GUI_PrepareFrame(MapData_t *map, renderitem_t *renderbuffer, Position_t *playerpos, framedata_t* frame){
    char i, count = 0;
    double val = 180/M_PI;
    int player_x = playerpos->coords.x>>8, player_y = playerpos->coords.y>>8, player_z = playerpos->coords.y>>8;
    int item_x, item_y, item_z;
    int distance_x, distance_y, distance_z;
    long distance;
    memset(renderbuffer, 0, sizeof(renderitem_t) * 20);
    for(i=0; i<20; i++){
        MapData_t *item = &map[i];
        item_x = item->position.coords.x>>8;
        item_y = item->position.coords.y>>8;
        item_z = item->position.coords.z>>8;
        distance_x = item_x - player_x;
        distance_y = item_y - player_y;
        distance_z = item_z - player_z;
        distance = r_GetDistance(distance_x, distance_y, distance_z);
        if(distance < RENDER_DISTANCE){
            unsigned char objectvect_xz = (unsigned char)(atan2(distance_z, distance_x) * val);
            unsigned char objectvect_y = (unsigned char)(atan2(distance_y, distance_x) * val);
            char vectordiff_xz, vectordiff_y;
            objectvect_xz = 255* objectvect_xz / 360;
            objectvect_y = 255* objectvect_y / 360;
            vectordiff_xz = playerpos->angles.xz - objectvect_xz;
            vectordiff_y = playerpos->angles.y - objectvect_y;
            if(abs(vectordiff_xz) <= 45 && abs(vectordiff_y) <= 45){
                int vcenter = vHeight>>1 + yStart;
                renderitem_t *render = &renderbuffer[count++];
                render->spriteid = item->entitytype;
                render->distance = RENDER_DISTANCE - distance;
                vectordiff_xz += 46;
                render->x = vWidth * vectordiff_xz / 91;
                vectordiff_y += 46;
                render->y = (vHeight * vectordiff_y / 91);
            }
        }
    }
    frame->count = count;
}


void GUI_RenderFrame(framedata_t *frame, renderitem_t *renderbuffer){
    char count = frame->count;
    char i;
    if(count){
        heapsort(renderbuffer, count);
        for(i = 0; i < count; i++){
            renderitem_t *render = &renderbuffer[i];
            if(render->spriteid>1){
                gfx_sprite_t* sprite = (gfx_sprite_t*)trekvfx[render->spriteid-2];
                gfx_sprite_t* uncompressed = gfx_MallocSprite(32, 32);
                gfx_sprite_t* scaled;
                char scale;
                zx7_Decompress(uncompressed, sprite);
                scale = render->distance * uncompressed->width / RENDER_DISTANCE;
                scaled = gfx_MallocSprite(scale, scale);
                scaled->width = scale;
                scaled->height = scale;
                gfx_ScaleSprite(uncompressed, scaled);
                free(uncompressed);
                gfx_TransparentSprite(scaled, render->x - (scaled->width>>1), render->y - (scaled->height>>1));
                free(scaled);
                //render->type; // use this to locate sprite
            }
        }
    }
}
