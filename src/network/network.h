#ifndef TITREK_NETWORK_H
#define TITREK_NETWORK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "../util.h"
#include "../flags.h"

typedef struct {
    bool network_up;
    bool bridge_up;
    bool bridge_error;
    bool client_version_ok;
    bool logged_in;
    bool game_loaded;
} net_flags_t;
extern net_flags_t netflags;


typedef struct {
    uint8_t control;
    uint8_t data[1];
} packet_t;

enum net_mode_id {
    MODE_SERIAL,
    MODE_CEMU_PIPE
};

typedef struct {
    uint8_t id;
    bool (*init)(void);
    void (*process)(void);
    bool (*read_to_size)(size_t size);
    void (*write)(void *data, size_t size);
} net_mode_t;

extern net_mode_t mode_srl;
extern net_mode_t mode_pipe;

extern char net_buf[8192];
extern size_t net_buf_size;

bool ntwk_init(void);
void ntwk_process(void);

bool ntwk_send_(uint8_t num_parts, uint8_t ctrl, ...);

#define PS_STR(str) (str), 1+strlen(str)
#define PS_VAL(val) (&val), sizeof(val)
#define PS_ARR(arr) (arr), sizeof(arr)
#define PS_PTR(ptr, size) (ptr), (size)

#define ntwk_send(ctrl_code, ...) ntwk_send_(VA_NARGS(__VA_ARGS__) / 2, ctrl_code, __VA_ARGS__)
#define ntwk_send_nodata(ctrl_code) ntwk_send_(0, ctrl_code)

// Inbound
void conn_HandleInput(packet_t *in_buff, size_t buff_size);

#endif //TITREK_NETWORK_H
