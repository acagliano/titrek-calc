
#include <graphx.h>

#include "ship.h"
#include "controlcodes.h"
#include "network.h"
#include "gameloop.h"
#include "../gfx/TrekGFX.h"

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
    
}

void ship_render_sysextended(void* sysext){

}
