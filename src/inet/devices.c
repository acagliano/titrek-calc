#include <stdlib.h>

#include <libload.h>
#include <usbdrvce.h>
#include <srldrvce.h>

#include "../ev.h"
#include "../gamestate.h"
#include "devices.h"


#define INET_TCP 		0
#define INET_SERIAL 	1
#define INET_DEVICES	2

#define ntwk_process(data)	usb_HandleEvents()

struct _device_init_t {
	const usb_standard_descriptors_t* descriptors;
	usb_error_t (*usb_handler)();
};
struct _device_init_t device_idata[INET_DEVICES] = {
	{NULL, NULL},
	{srl_GetCDCStandardDescriptors, srl_handle_usb_event}
};

void ntwk_init(void){
	uint8_t inet_devices = 0;
	uint24_t err;

	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	inet_devices |= libload_IsLibLoaded(TCPDRVCE);
	inet_devices |= (libload_IsLibLoaded(SRLDRVCE)<<1);
	if (!inet_devices) exit(ERR_IFACE_NOT_FOUND);
	
	gamestate.inet_data.inet_process = usb_HandleEvents;
	
	for(uint8_t i=0; i<INET_DEVICES;i++){
		if (inet_devices>>i) {
			if (usb_Init(device_idata[i].usb_handler,
					 NULL,
					 device_idata[i].descriptors,
					 USB_DEFAULT_INIT_FLAGS))
				exit(ERR_USBDRV_ERROR);
			enqueue(gamestate.inet_data.inet_process, true);
			return;
		}
	}
}

