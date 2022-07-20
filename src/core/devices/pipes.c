#include <stdbool.h>
#include <stdio.h>
#include <fontlibc.h>
#include <graphx.h>

#include "../../asm/exposure.h"
#include "../network.h"

#define CEMU_CONSOLE ((char*)0xFB0000)

bool pipe_read_to_size(size_t size);

net_mode_t mode_pipe = {
        MODE_CEMU_PIPE,
        NULL,
        pipe_read_to_size,
        cemu_send
};

bool pipe_init(void){
    fontlib_DrawString("init cemu pipes... ");
    gfx_BlitBuffer();
    if(!cemu_check()) return false;
    mode = &mode_pipe;
    return true;
}

bool pipe_read_to_size(size_t size) {
    static size_t net_buf_size;
    if(net_buf_size < size) {
        size_t recd;
        recd = cemu_get(&net_parse_buffer[net_buf_size], size - net_buf_size);
        net_buf_size += recd;
    }

    if(net_buf_size > size) {
        sprintf(CEMU_CONSOLE, "Pipe buffer in illegal state\nYou may want to restart the network.\n");
    }

    if(net_buf_size == size) {
        net_buf_size = 0;
        return true;
    }

    return false;
}

