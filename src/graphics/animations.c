#include <sys/util.h>
#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <compression.h>
#include "../gfx/internal.h"
#include "uiflags.h"

struct _starfield {
    uint8_t size;
    uint24_t x;
    uint8_t y;
};

#define STAR_OBJ_COUNT 50
#define STAR_SPRITE_COUNT 3
#define STAR_SPRITE_SIZE (8*8)
#define SMALL_STAR_IDX 0
#define MEDIUM_STAR_IDX 1
#define LARGE_STAR_IDX 2
#define STAR_ANIM_SPEED 5
void anim_MainMenu(uint24_t x_start, uint8_t y_start, uint24_t x_Width, uint8_t y_Height){
    static struct _starfield star_entities[STAR_OBJ_COUNT];
    static bool starfield_init = false;
    static uint8_t star_sprites[STAR_SPRITE_COUNT][STAR_SPRITE_SIZE];
    static uint24_t tick=STAR_ANIM_SPEED;
    gfx_rletsprite_t *small_star = (gfx_rletsprite_t*)star_sprites[SMALL_STAR_IDX],
                    *medium_star = (gfx_rletsprite_t*)star_sprites[MEDIUM_STAR_IDX],
                    *large_star = (gfx_rletsprite_t*)star_sprites[LARGE_STAR_IDX],
                    *this_star;
    
    if(!starfield_init){
        for(int i=0; i<50; i++){
            star_entities[i].x = randInt(x_start, x_start+x_Width);
            star_entities[i].y = randInt(y_start, y_start+y_Height);
            if(i<30) star_entities[i].size = 0;
            else if(i<44) star_entities[i].size = 1;
            else star_entities[i].size = 2;
        }
        zx7_Decompress(small_star, star_small_compressed);
        zx7_Decompress(medium_star, star_medium_compressed);
        zx7_Decompress(large_star, star_large_compressed);
        starfield_init = true;
    }
    
    if(!tick) {
        gfx_ZeroScreen();
        gfx_SetClipRegion(x_start, y_start, x_start+x_Width, y_start+y_Height);
        for(int i=0; i<50; i++){
            this_star = (gfx_rletsprite_t*)star_sprites[star_entities[i].size];
            gfx_RLETSprite(this_star, star_entities[i].x, star_entities[i].y);
            star_entities[i].x -= (star_entities[i].size+1);
            if(star_entities[i].x > 320) {
                star_entities[i].y = randInt(y_start, y_start+y_Height);
                star_entities[i].x = 320;
            }
        }
        tick = STAR_ANIM_SPEED;
        splash_render = true;
    }
        
    else tick--;
}
