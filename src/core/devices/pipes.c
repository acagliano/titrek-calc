#include <stdbool.h>

#include "../../asm/exposure.h"

net_mode_t mode_pipe = {
        MODE_CEMU_PIPE,
        pipe_init,
        NULL,
        pipe_read_to_size,
        cemu_send
};


bool pipes_init(void){
    if(!cemu_check()) return false;
    
    mode = &mode_pipe;
}

bool pipe_read_to_size(size_t size) {
    if(net_buf_size < size) {
        size_t recd;
        recd = cemu_get(&net_buf[net_buf_size], size - net_buf_size);
        net_buf_size += recd;
    }

    if(net_buf_size > size) {
        dbg_printf("Pipe buffer in illegal state\n");
    }

    if(net_buf_size == size) {
        net_buf_size = 0;
        return true;
    }

    return false;
}
