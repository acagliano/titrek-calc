#include <tice.h>
#include "particles.h"
#include "../equates.h"
#include "../gfx/TrekGFX.h"

void gfx_RenderParticles(particles_t* part, uint8_t ct){
    uint8_t i, y;
    uint24_t x;
    for(i = 0; i < ct; i++){
        particles_t* p = &part[i];
        switch(p->type){
            case NONE:
                break;
            case CRACKED_SCREEN:
                gfx_RLETSprite(cracked, p->x, p->y);
                break;
        }
    }
}

bool gfx_SetParticle(particles_t* part, uint8_t ct, uint8_t type){
    uint8_t i;
    bool success = false;
    for(i = 0; i < ct; i++){
        particles_t* p = &part[i];
        if(!p->type){
            success = true;
            p->type = type;
            switch(type){
                case CRACKED_SCREEN:
                    p->x = randInt(xStart + 16, xStart + vWidth - 32);
                    p->y = randInt(yStart + 16, yStart + vHeight - 32);
                    p->timeout = -1;
                    break;
                default:
                    p->timeout = 100;
            }
            break;
        }
    }
    return success;
}

void gfx_ClearParticles(particles_t* part, uint8_t ct){
    uint8_t i;
    for(i = 0; i < ct; i++){
        particles_t* p = &part[i];
        p->type = NONE;
    }
}
