#include <stdlib.h>

#include <libload.h>
#include <usbdrvce.h>
#include <srldrvce.h>

#include "../ev.h"
#include "../gamestate.h"
#include "devices.h"


#define INET_TCP 		0
#define INET_SERIAL 	1
#define INET_DEVICECT	2

#define ntwk_process(data)	usb_HandleEvents()
usb_device_t usb_device = NULL;
uint8_t net_buffer[NET_BUFFER_SIZE];

struct _device_init_t {
	const usb_standard_descriptors_t* descriptors;
	usb_error_t (*usb_handler)();
};
struct _device_init_t device_idata[INET_DEVICECT] = {
	{NULL, NULL},
	{srl_GetCDCStandardDescriptors, srl_handle_usb_event}
};

void ntwk_init(void){
	uint8_t inet_devices = 0;

	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	inet_devices |= libload_IsLibLoaded(TCPDRVCE);
	inet_devices |= (libload_IsLibLoaded(SRLDRVCE)<<1);
	if (!inet_devices) exit(ERR_IFACE_NOT_FOUND);
	
	gamestate.inet_data.inet_process = usb_HandleEvents;
	
	for(uint8_t i=0; i<INET_DEVICECT;i++){
		if ((inet_devices>>i) & 1) {
			if (usb_Init(device_idata[i].usb_handler,
					 NULL,
					 device_idata[i].descriptors,
					 USB_DEFAULT_INIT_FLAGS))
				exit(ERR_USBDRV_ERROR);
			enqueue(gamestate.inet_data.inet_process, PROC_NTWK, true);
			return;
		}
	}
}


// SERIAL
srl_device_t srl;

bool serial_open(void){
	if(!srl_Open(&srl, usb_device, net_buffer, sizeof net_buffer, SRL_INTERFACE_ANY, 115200)){
		gamestate.inet_data.inet_flags |= (1<<INET_ACTIVE);
		gamestate.inet_data.inet_flags |= (DEVICE_SRL<<INET_DEVICE);
		MARK_FRAME_DIRTY;
		gamestate.inet_data.inet_recv = srl_Read;
		gamestate.inet_data.inet_send = srl_Write;
		gamestate.inet_data.inet_setdown = srl_Close;
		return true;
	}
	return false;
}

static usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
										usb_callback_data_t *callback_data) {
	
	usb_error_t err;
	
	/* Delegate to srl USB callback */
	if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS) return err;
	
	/* Enable newly connected devices */
	switch(event){
		case USB_DEVICE_CONNECTED_EVENT:
			if (!(usb_GetRole() & USB_ROLE_DEVICE)){
				usb_device_t usb_device = event_data;
				usb_ResetDevice(usb_device);
			}
			break;
		case USB_HOST_CONFIGURE_EVENT:
		{
			usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
			if(host) usb_device = host;
			if(serial_open()) return USB_SUCCESS;
			break;
		}
			// break;
		case USB_DEVICE_ENABLED_EVENT:
			usb_device = event_data;
			if(serial_open()) return USB_SUCCESS;
			break;
			// break;
		case USB_DEVICE_DISCONNECTED_EVENT:
			srl_Close(&srl);
			gamestate.inet_data.inet_flags &= ~(1<<INET_ACTIVE);
			return USB_SUCCESS;
			
	}
	return USB_SUCCESS;
}

