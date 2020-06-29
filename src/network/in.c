#include <keypadc.h>
#include <stdbool.h>
#include "../equates.h"
#include "../rendering/errors.h"
#include "../classes/settings.h"
#include "../classes/player.h"
#include "controlcodes.h"
#include "network.h"
#include "../rendering/gui.h"
#include "../rendering/engine.h"

void conn_HandleInput(packet_t *in_buff, size_t buff_size) {
    uint8_t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    
    switch(ctl){
        case REGISTER:
        case LOGIN:
            if(response == SUCCESS) {gameflags.logged_in = true; break;}
            if((response == MISSING) && (ctl == LOGIN)) gui_Register();
            else {
                memset(&settings.userinfo, 0, sizeof(userinfo_t));
                gui_NetworkErrorResponse(ctl, response, true);
            }
            break;
        case DISCONNECT:
            gameflags.logged_in = false;
            break;
        case REQCHUNK:
            renderFrame((body_packet_t*)data);
            break;
        case MESSAGE:
            // to handle
            break;
        default:
            gui_NetworkErrorResponse(3, 7, true);
    }
}


