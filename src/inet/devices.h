#ifndef devices_h
#define devices_h

#include <usbdrvce.h>

#define NET_BUFFER_SIZE	4096
extern usb_device_t usb_device;
extern net_buffer[NET_BUFFER_SIZE];

// inet_flags is a series of bits that indicate network status/config
#define INET_ACTIVE		0
#define INET_CONNECTED	1
#define INET_ENABLE_ENCRYPTION 7
extern bool inet_flags;

struct inet_data_t {
	bool inet_flags;
	void (*inet_process)();
	void (*inet_recv)();
	void (*inet_send)();
	void (*inet_setdown)();
};

// main network init caller
void ntwk_init(void);

// Serial event callback handler
static usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
										usb_callback_data_t *callback_data);

// TCP event callback handler

#endif
