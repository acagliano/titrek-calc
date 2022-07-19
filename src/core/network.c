#include <stdbool.h>
#include <string.h>

#include "../graphics/console.h"
#include "network.h"
#include "gameloop.h"
#include "settings.h"

uint8_t net_device_buffer[NTWK_BUFFER_SIZE];
uint8_t net_parse_buffer[NTWK_BUFFER_SIZE>>1];
uint8_t packet_queue_buffer[NTWK_BUFFER_SIZE>>1] = {0};
packet_queue_t *packet_queue = (packet_queue_t*)packet_queue_buffer;

net_mode_t *mode;
network_error_t network_error = NTWK_OK; 

bool ntwk_init(void) {
    if(tcp_init()) return true;
    else if(serial_init()) return true;
    else if(pipe_init()) return true;
    
    console_write(ENTRY_ERROR_MSG, "No network device available or init error. Check libraries.");
}

void ntwk_process(void) {
    uint8_t i;
    static size_t packet_size = 0;

    for(i = 0; i < 2 * settings.packet_limit.current; i++ ) {
        if(mode->process) mode->process();

        /* If the device was disconnected, exit */
        if(tick_loop_mode == NO_CONNECTION) return;

        /* Handle input */
        if(packet_size) {
            if(mode->read_to_size(packet_size)) {
                conn_HandleInput((packet_t *)net_parse_buffer, packet_size);
                packet_size = 0;
            } else break;
        } else {
            if(mode->read_to_size(sizeof(packet_size))) packet_size = *(size_t*)net_parse_buffer;
            else break;
        }
    }
}

bool ntwk_queue(void* data, size_t len){
    if((packet_queue->size) + len > (NTWK_BUFFER_SIZE>>1)) return false;
    memcpy(&packet_queue->data[packet_queue->size], data, len);
    packet_queue->size += len;
    return true;
}


bool ntwk_send(void) {

    mode->write(packet_queue->size, sizeof(packet_queue->size));
    mode->write(packet_queue->data, packet_queue->size);
    packet_queue->size = 0;

    return true;
}

