#include <stdarg.h>
#undef NDEBUG
#include <debug.h>
#include "network.h"
#include "../equates.h"
#include "usb.h"
#define PACKET_LIMIT 5

bool ntwk_init(void) {
    gameflags.network = false;
    return init_usb();
}

void ntwk_process(void) {
    static size_t current_size = 0;
    uint8_t i;
    static char in_buff[1024];
    for(i = 0; i < PACKET_LIMIT && srl_Available(&srl); i++){
        usb_HandleEvents();

        /* If the device was disconnected, exit */
        if(!gameflags.network) return;

        /* Handle input */
        if(current_size) {
            if(srl_Available(&srl) >= current_size) {
                srl_Read(&srl, in_buff, current_size);
                conn_HandleInput((packet_t *) &in_buff, current_size);
                current_size = 0;
            }
        } else {
            if(srl_Available(&srl) >= sizeof(current_size)) srl_Read(&srl, (void*)&current_size, sizeof(current_size));
        }
    }
}

bool ntwk_send_(uint8_t num_parts, uint8_t ctrl, ...) {
    uint8_t i;
    va_list ap;
    size_t total_size = 1;

    if(!gameflags.network) return false;

    va_start(ap, num_parts);
    for(i = 0; i < num_parts; i++) {
        va_arg(ap, void*);
        total_size += va_arg(ap, size_t);
    }
    va_end(ap);

    dbg_sprintf(dbgout, "Total size: %u\n", total_size);

    srl_Write(&srl, &total_size, sizeof(total_size));

    srl_Write(&srl, &ctrl, sizeof(ctrl));

    va_start(ap, num_parts);
    for(i = 0; i < num_parts; i++) {
        void *data = va_arg(ap, void*);
        size_t size = va_arg(ap, size_t);

        srl_Write(&srl, data, size);
    }
    va_end(ap);

    return true;
}
