
#include <graphx.h>

#include "ship.h"
#include "controlcodes.h"
#include "network.h"
#include "../gfx/TrekGFX.h"

ship_system_data_t ship_systems[SHIP_MAX_MODULES];

void ship_render_interior(void){

}


void ship_load_info(void){
    if(!TrekGFX_init()){
        game_error = GFX_LOAD_ERROR;
        return;
    }
    uint8_t outctl = LOAD_SHIP;
    ntwk_queue(&outctl, sizeof outctl);
    ntwk_send();
}
