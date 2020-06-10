#include <keypadc.h>
#include <stddef.h>
#include <stdbool.h>
#include <srldrvce.h>
#include "../equates.h"
#include "../errorscreens.h"
#include "usb.h"
#include "controlcodes.h"


void conn_HandleInput(usb_packet_t *in_buff, size_t buff_size) {
    uint8_t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    switch(ctl){
        case REGISTER:
        case LOGIN:
            if(response == SUCCESS) gameflags.logged_in = true;
            if((response == MISSING) && (ctl == LOGIN)) ntwk_Register(&in_buff->data[1], buff_size - 2);
            else {
                gui_NetworkErrorResponse(ctl, response);
                while(!kb_AnyKey());
            }
            break;
        case DISCONNECT:
            gameflags.logged_in = false;
            break;
        case MESSAGE:
            // to handle
            break;
    }
}


