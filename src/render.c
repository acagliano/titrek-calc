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
        distance = (long)sqrt(r_GetDistance(distance_x, distance_y, distance_z));
        if(distance < RENDER_DISTANCE){
            char objectvect_xz = byteATan(distance_z, distance_x);
            char objectvect_y = byteATan(distance_y, distance_x);
            char diff_xz = objectvect_xz - playerpos->angles.xz;
            char diff_y = objectvect_y - playerpos->angles.y;
            //objectvect_xz = AngleOpsBounded(objectvect_xz, -1 * playerpos->angles.xz);
            //objectvect_y = AngleOpsBounded(objectvect_y, -1 * playerpos->angles.y);
            //vectordiff_xz = AngleOpsBounded(objectvect_xz, playerpos->angles.xz);
           // vectordiff_y = AngleOpsBounded(objectvect_y, playerpos->angles.y);
            if((abs(diff_xz) <= 9) && (abs(diff_y) <= 9)){
                int vcenter = vHeight>>1 + yStart;
                renderitem_t *render = &renderbuffer[count++];
                render->spriteid = item->entitytype-1;
                render->distance = (RENDER_DISTANCE - distance) * 100 / RENDER_DISTANCE;
                diff_xz += 10;
                render->x = vWidth * diff_xz / 19;
                diff_y += 10;
                render->y = (vHeight * diff_y / 19);
            }
        }
    }
    frame->count = count;
    if(count){
        heapsort(renderbuffer, count);
        for(i = 0; i < count; i++){
            renderitem_t *render = &renderbuffer[i];
            if(render->spriteid){
                gfx_sprite_t* sprite = (gfx_sprite_t*)trekvfx[render->spriteid];
                gfx_sprite_t* uncompressed;
                gfx_sprite_t* scaled;
                char scale = render->distance;
                int width, height;
                switch(render->spriteid){
                    case et_ship-1:
                        uncompressed = gfx_MallocSprite(64, 32);
                        break;
                    default:
                        uncompressed = gfx_MallocSprite(32, 32);
                }
                zx7_Decompress(uncompressed, sprite);
                //if(scale != -1){
                width = uncompressed->width * scale / 100;
                height = uncompressed->height * scale / 100;
                scaled = gfx_MallocSprite(width, height);
                scaled->width = width;
                scaled->height = height;
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
