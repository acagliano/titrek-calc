#include <stddef.h>
#include <stdbool.h>
#include <srldrvce.h>
#include "../equates.h"
#include "../errorscreens.h"
#include "usb.h"
#include "controlcodes_in.h"

void conn_HandleInput(usb_packet_t* in_buff, flags_t* gameflags){
    uint8t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    switch(ctl){
        case REGISTER:
        case LOGIN:
            if(response == RL_SUCCESS) gameflags->logged_in = true;
            if((response == RL_MISSING) && (ctl == LOGIN)) ntwk_Register(&in_buff->data[1]);
            else gui_NetworkErrorResponse(ctl, response);
            break;
        case DISCONNECT:
            gameflags->logged_in = false;
            break;
        case RESP_MESSAGE:
            // to handle
            break;
        case RESP_DEBUG:
            // to handle
            break;
    }
}


