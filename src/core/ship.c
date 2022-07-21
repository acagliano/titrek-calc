
#include <graphx.h>
#include <fontlibc.h>

#include "ship.h"
#include "controlcodes.h"
#include "network.h"
#include "gameloop.h"
#include "../gfx/TrekGFX.h"

#define num_InRange(min, max, num)  \
        (((min) < (num)) && ((num) < (max)))

ship_system_data_t ship_systems[SHIP_MAX_MODULES];

void ship_get_info(void){
    if(!TrekGFX_init()){
        game_error = GFX_LOAD_ERROR;
        return;
    }
    uint8_t outctl = LOAD_SHIP;
    ntwk_queue(&outctl, sizeof outctl);
    ntwk_send();
}

void ship_render_interior(void){
    gfx_RLETSprite(shipinterior_left, 0, 0);
    gfx_RLETSprite(shipinterior_right, (320>>1), 0);
}

void ship_render_sysoverview(ship_system_data_t* sys){
    uint24_t xstart = 50;
    uint8_t ystart = 30;
    for(int i=0; i<SHIP_MAX_MODULES; i++){
        ship_system_data_t *s = &sys[i];
        
        // if system unset, we've hit the end of the list
        if(!s->system_type) break;
        
        // system "classes" cover types within a 64-type space
        // we may not even split by class, there's enough space on screen to render em all
        // if(!num_InRange(6<<mode, 6<<(mode+1), sys->system_type)) continue;
        
        gfx_RLETSprite((gfx_rletsprite_t*)sys->icon, xstart, 10*i+ystart);
        fontlib_SetCursorPosition(xstart + 10, 10*i+ystart);
        fontlib_DrawString(sys->system_name);
    }
    gfx_BlitBuffer();
}

void ship_render_sysextended(void* sysext){

}
