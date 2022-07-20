
#include <stdbool.h>
#include <fontlibc.h>
#include <graphx.h>
#include "../network.h"

uint8_t *tcp_buf = net_device_buffer;

net_mode_t mode_tcp = {
        MODE_TCP,
        NULL,
        NULL,
        NULL
};

bool tcp_init(void){
    fontlib_DrawString("init tcp device... ");
    gfx_BlitBuffer();
    return false;
    // placeholder code for tcp library
}
