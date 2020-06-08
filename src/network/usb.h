#ifndef usb_h
#define usb_h

typedef struct {
    uint8_t control;
    uint8_t data[1];
} usb_packet_t;

void conn_HandleInput(usb_packet_t* in_buff, flags_t* gameflags);

#endif
