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
    char i, count = 0, scale;
    double val = 180/M_PI;
    unsigned long player_x = playerpos->coords.x;
    unsigned long player_y = playerpos->coords.y;
    unsigned long player_z = playerpos->coords.y;
    unsigned long item_x, item_y, item_z;
    long distance_x, distance_y, distance_z;
    unsigned long distance;
    memset(renderbuffer, 0, sizeof(renderitem_t) * 20);
    for(i=0; i<20; i++){
        MapData_t *item = &map[i];
        item_x = item->position.coords.x;
        item_y = item->position.coords.y;
        item_z = item->position.coords.z;
        distance_x = item_x - player_x;
        distance_y = item_y - player_y;
        distance_z = item_z - player_z;
        distance = r_GetDistance(distance_x, distance_y, distance_z);
        if(distance < RENDER_DISTANCE){
            unsigned int objectvect_xz = (unsigned char)(atan2(distance_z, distance_x) * val / 5);
            unsigned int objectvect_y = (unsigned char)(atan2(distance_y, distance_x) * val / 5);
            char vectordiff_xz, vectordiff_y;
            objectvect_xz = (objectvect_xz >= 0) ? objectvect_xz : 36 - objectvect_xz;
            if(objectvect_xz > 71) objectvect_xz = 0;
            objectvect_y = (objectvect_y >= 0) ? objectvect_y : 36 - objectvect_y;
            if(objectvect_y > 71) objectvect_y = 0;
            vectordiff_xz = AngleOpsBounded(objectvect_xz, playerpos->angles.xz);
            vectordiff_y = AngleOpsBounded(objectvect_y, playerpos->angles.y);
            if(abs(vectordiff_xz) <= 9 && abs(vectordiff_y) <= 9){
                int vcenter = vHeight>>1 + yStart;
                renderitem_t *render = &renderbuffer[count++];
                render->spriteid = item->entitytype;
                render->distance = calcSpriteScale(distance, RENDER_DISTANCE);
                vectordiff_xz += 10;
                render->x = vWidth * vectordiff_xz / 19;
                vectordiff_y += 10;
                render->y = (vHeight * vectordiff_y / 19);
            }
        }
    }
    frame->count = count;
    if(count){
        heapsort(renderbuffer, count);
        for(i = 0; i < count; i++){
            renderitem_t *render = &renderbuffer[i];
            if(render->spriteid){
                gfx_sprite_t* sprite = (gfx_sprite_t*)trekvfx[render->spriteid-1];
                gfx_sprite_t* uncompressed;
                gfx_sprite_t* scaled;
                char scale = render->distance;
                switch(render->spriteid){
                    case et_ship:
                        uncompressed = gfx_MallocSprite(64, 32);
                        break;
                    default:
                        uncompressed = gfx_MallocSprite(32, 32);
                }
                zx7_Decompress(uncompressed, sprite);
                //if(scale != -1){
                scaled = gfx_MallocSprite(scale, scale);
                scaled->width = scale;
                scaled->height = scale;
                gfx_ScaleSprite(uncompressed, scaled);
                gfx_TransparentSprite(scaled, render->x - (scaled->width>>1), render->y - (scaled->height>>1));
                free(scaled);
                // }
                free(uncompressed);
                //render->type; // use this to locate sprite
            }
        }
    }
}

/*
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
                char scale = render->distance;
                zx7_Decompress(uncompressed, sprite);
                //if(scale != -1){
                    scaled = gfx_MallocSprite(scale, scale);
                    scaled->width = scale;
                    scaled->height = scale;
                    gfx_ScaleSprite(uncompressed, scaled);
                    gfx_TransparentSprite(scaled, render->x - (scaled->width>>1), render->y - (scaled->height>>1));
                    free(scaled);
               // }
                free(uncompressed);
                //render->type; // use this to locate sprite
            }
        }
    }
}*/
