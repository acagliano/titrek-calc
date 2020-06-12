#ifndef usb_h
#define usb_h

typedef struct {
    uint8_t control;
    uint8_t data[1];
} usb_packet_t;

// Inbound
void conn_HandleInput(usb_packet_t *in_buff, size_t buff_size);

// Outbound
bool ntwk_Login(void);
bool ntwk_Register(void);
void ntwk_Disconnect(void);

#endif
