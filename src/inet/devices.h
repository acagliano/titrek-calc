#ifndef devices_h
#define devices_h

#include <usbdrvce.h>
#include <srldrvce.h>

#define NET_BUFFER_SIZE	4096
extern usb_device_t usb_device;
extern uint8_t net_buffer[NET_BUFFER_SIZE];

#define INET_DEVICECT	2
typedef struct _device_init_t {
	const usb_standard_descriptors_t* descriptors;
	usb_error_t (*usb_handler)();
} device_init_t;
extern device_init_t device_idata[INET_DEVICECT];

// Serial event callback handler
usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
										usb_callback_data_t *callback_data);

// TCP event callback handler

#endif
