#include <usbdrvce.h>
#include <srldrvce.h>
#include <stdbool.h>
#include "../devices.h"
#include "../../gamestate.h"

//bool serial_read_to_size(size_t size);
//void serial_write(void *buf, size_t size);


srl_device_t srl;


/*bool serial_read_to_size(size_t size){
    static size_t bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_parse_buffer[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void serial_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}
*/
bool serial_open(void){
	if(!srl_Open(&srl, usb_device, net_buffer, sizeof net_buffer, SRL_INTERFACE_ANY, 115200)){
		gamestate.inet_data.inet_flags |= (1<<INET_ACTIVE);
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
		}
			// break;
		case USB_DEVICE_ENABLED_EVENT:
			usb_device = event_data;
			if(serial_open()) return USB_SUCCESS;
			// break;
		case USB_DEVICE_DISCONNECTED_EVENT:
			srl_Close(&srl);
			gamestate.inet_data.inet_flags &= ~(1<<INET_ACTIVE);
			return USB_SUCCESS;
				
	}
    return USB_SUCCESS;
}
