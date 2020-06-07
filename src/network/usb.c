#include <stddef.h>
#include <stdbool.h>
#include <srldrvce.h>
#include "usb.h"
#include "controlcodes.h"

void conn_HandleInput(usb_packet_t* in_buff){
    uint24_t ctl_code = in_buff->control;
}
