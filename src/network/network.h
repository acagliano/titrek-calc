#ifndef TITREK_NETWORK_H
#define TITREK_NETWORK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../util.h"

typedef struct {
    uint8_t control;
    uint8_t data[1];
} packet_t;

bool ntwk_init(void);
void ntwk_process(void);

bool ntwk_send_(uint8_t num_parts, uint8_t ctrl, ...);

#define PS_STR(str) (str), 1+strlen(str)
#define PS_VAL(val) (val), sizeof(val)
#define PS_ARR(arr) (arr), sizeof(arr)
#define PS_PTR(ptr, size) (ptr), (size)

#define ntwk_send(ctrl_code, ...) ntwk_send_(VA_NARGS(__VA_ARGS__) / 2, ctrl_code, __VA_ARGS__)
#define ntwk_send_nodata(ctrl_code) ntwk_send_(0, ctrl_code)

// Inbound
void conn_HandleInput(packet_t *in_buff, size_t buff_size);

#endif //TITREK_NETWORK_H
