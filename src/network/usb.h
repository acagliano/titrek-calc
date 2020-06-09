#ifndef usb_h
#define usb_h

typedef struct {
    uint8_t control;
    uint8_t data[1];
} usb_packet_t;

// Inbound
void conn_HandleInput(usb_packet_t* in_buff, size_t buff_size, flags_t* gameflags);

// Outbound
void ntwk_Login(flags_t* flags);
void ntwk_Register(uint8_t* loginstuff, size_t buff_size);
void ntwk_Disconnect(void);

#endif
