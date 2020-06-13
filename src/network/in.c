#include <keypadc.h>
#include <stdbool.h>
#include "../equates.h"
#include "../errorscreens.h"
#include "controlcodes.h"
#include "network.h"
#include "../gfx-engine/gui.h"

void conn_HandleInput(packet_t *in_buff, size_t buff_size) {
    uint8_t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    
    switch(ctl){
        case REGISTER:
        case LOGIN:
            if(response == SUCCESS) gameflags.logged_in = true;
            if((response == MISSING) && (ctl == LOGIN)) gui_Register();
            else gui_NetworkErrorResponse(ctl, response, true);
            break;
        case DISCONNECT:
            gameflags.logged_in = false;
            break;
        case MESSAGE:
            // to handle
            break;
        default:
            gui_NetworkErrorResponse(3, 7, true);
    }
}


