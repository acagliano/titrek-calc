#include <stdbool.h>
#include <debug.h>
#include "../asm/exposure.h"
#include "network.h"
#include "../equates.h"

char net_buf[1024];
size_t net_buf_size = 0;

bool pipe_init() {
    gameflags.network = true;
    return true;
}

bool pipe_read_to_size(size_t size) {
    if(net_buf_size < size) {
        size_t recd;
        recd = cemu_get(&net_buf[net_buf_size], size - net_buf_size);
        net_buf_size += recd;
    }

    if(net_buf_size > size) {
        dbg_sprintf(dbgerr, "Pipe buffer in illegal state\n");
    }

    if(net_buf_size == size) {
        net_buf_size = 0;
        return true;
    }

    return false;
}

net_mode_t mode_pipe = {
        MODE_CEMU_PIPE,
        pipe_init,
        NULL,
        pipe_read_to_size,
        cemu_send
};
