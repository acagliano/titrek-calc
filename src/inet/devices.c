
#include <libload.h>
#include <usbdrvce.h>
#include <srldrvce.h>

#include "../../ev.h"
#include "devices.h"


#define INET_TCP 		0
#define INET_SERIAL 	1
#define INET_DEVICES	2

#define ntwk_process(data)	usb_HandleEvents()

usb_standard_descriptors_t* descriptors[] = {
	NULL,
	srl_GetCDCStandardDescriptors()
};

usb_error_t (*usb_handlers)()[] = {
	NULL,
	srl_handle_usb_event
};

void ntwk_init(void){
	uint8_t inet_devices = 0;
	uint24_t err;
	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	inet_devices |= libload_IsLibLoaded(TCPDRVCE);
	inet_devices |= (libload_IsLibLoaded(SRLDRVCE)<<1);
	if (!inet_devices) exit(ERR_IFACE_NOT_FOUND);
	
	gameflags.inet_data.inet_process = usb_HandleEvents;
	
	for(uint8_t i=0; i<INET_DEVICES;i++){
		if (inet_devices>>i) {
			if (usb_Init(usb_handlers[i],
					 NULL,
					 descriptors[i],
					 USB_DEFAULT_INIT_FLAGS))
				exit(ERR_USBDRV_ERROR);
			enqueue(gameflags.inet_data.inet_process, NULL, true);
			return;
		}
	}
}

