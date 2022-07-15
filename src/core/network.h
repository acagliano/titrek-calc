
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

bool ntwk_init(void);       // initialize devices in preference order tcp, serial, pipes
void ntwk_process(void);    // process usb events
bool ntwk_queue(void* data, size_t len);    // write partial frame to packet queue
bool ntwk_send(void);                       // send packet queue and reset queue

typedef struct {
    uint8_t control;
    uint8_t data[1];
} packet_t;

typedef struct _packet_queue_t {
    size_t size;
    uint8_t data[1];
} packet_queue_t;

typedef struct _hostinfo_t {
    char fname[9];
    char hostname[128];
} hostinfo_t;
extern hostinfo_t hostinfo;

typedef enum _network_error_t {
    NTWK_OK,
    NTWK_DEVICE_INIT_ERR,
    NTWK_BRIDGE_ERR,
    NTWK_SERVER_DISCONNECT,
    NTWK_TIMEOUT,
    NTWK_AUTH_ERR,
    NTWK_PEER_DISCONNECT = 0xf0
} network_error_t;
extern network_error_t network_error;

#define AES_KEYLEN 32
extern uint8_t aes_key[AES_KEYLEN];

extern uint24_t ntwk_timeout_clock;
#define ntwk_GetTimeoutEnabled() (ntwk_timeout_clock>>23)
#define ntwk_EnableTimeout() ntwk_timeout_clock |= 0x800000
#define ntwk_DisableTimeout() ntwk_timeout_clock &= 0x7fffff
#define ntwk_SetTimeout(timeout) ntwk_timeout_clock |= timeout
#define ntwk_GetTimerValue() ntwk_timeoutclock & 0x00ffff


/* Callers for Device Initializers */

bool init_usb(void);
bool tcp_init(void);
bool serial_init(void);

// usb process events
void usb_process(void);

#endif
