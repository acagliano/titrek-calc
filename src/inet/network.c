#include <stdbool.h>
#include <string.h>
#include <fontlibc.h>
#include <stdio.h>

#include "../graphics/console.h"
#include "network.h"
#include "gameloop.h"
#include "settings.h"

uint8_t net_device_buffer[NTWK_BUFFER_SIZE];
uint8_t net_parse_buffer[NTWK_BUFFER_SIZE>>1];
uint8_t packet_queue_buffer[NTWK_BUFFER_SIZE>>1] = {0, 0};
packet_queue_t *packet_queue = (packet_queue_t*)packet_queue_buffer;

net_mode_t *mode;
network_error_t network_error = NTWK_OK;
uint24_t ntwk_timeout_clock = 0;
uint8_t net_device = 0xff;

bool ntwk_init(void) {
    if(net_device == 0xff) tcp_init();
    if(net_device == 0xff) { fontlib_DrawString("failed\n"); serial_init(); }
    if(net_device == 0xff) { fontlib_DrawString("failed\n"); pipe_init(); }
    if(net_device == 0xff) return false;
    tick_loop_mode = DEVICE_UP;
    return true;
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

#define CEMU_CONSOLE ((char*)0xFB0000)
bool ntwk_send(void) {

    sprintf(CEMU_CONSOLE, "Sending a %u-byte size word: %u\n", sizeof(size_t), packet_queue->size);
    mode->write(&packet_queue->size, sizeof(size_t));
    sprintf(CEMU_CONSOLE, "Sending %u bytes of data.\n", packet_queue->size);
    mode->write(packet_queue->data, packet_queue->size);
    packet_queue->size = 0;

    return true;
}

