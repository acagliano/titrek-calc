#include <stdlib.h>
#include <stdio.h>
#include <libload.h>
#include <usbdrvce.h>
#include <srldrvce.h>

#include "../ev.h"
#include "../gamestate.h"
#include "devices.h"
#include "inet.h"

usb_device_t usb_device = NULL;
uint8_t net_buffer[NET_BUFFER_SIZE];

#define INET_TCP 		0
#define INET_SERIAL 	1

// SERIAL
srl_device_t srl;

#define CEMU_CONSOLE ((char*)0xFB0000)
bool serial_open(void){
	if(!srl_Open(&srl, usb_device, net_buffer, sizeof net_buffer, SRL_INTERFACE_ANY, 115200)){
		SET_FLAG(gamestate.inet.flags, INET_ACTIVE);
		gamestate.inet.device_id = DEVICE_SRL;
		MARK_FRAME_DIRTY;
		gamestate.inet.recv = srl_Read;
		gamestate.inet.send = srl_Write;
		gamestate.inet.setdown = srl_Close;
		return true;
	}
	return false;
}

usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
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
			RESET_FLAG(gamestate.inet.flags, INET_ACTIVE);
			return USB_SUCCESS;
			
	}
	return USB_SUCCESS;
}

