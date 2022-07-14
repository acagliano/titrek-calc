
#ifndef network_h
#define network_h

#define NTWK_BUFFER_SIZE 5120
extern uint8_t net_device_buffer[NTWK_BUFFER_SIZE];
extern uint8_t net_parse_buffer[NTWK_BUFFER_SIZE>>1];
extern uint8_t packet_queue_buffer[NTWK_BUFFER_SIZE>>1];

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

typedef struct _packet_queue_t {
    size_t size;
    uint8_t data[1];
} packet_queue_t;

#define AES_KEYLEN 32
extern uint8_t aes_key[AES_KEYLEN];

#endif
