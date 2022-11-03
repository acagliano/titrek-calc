#include <usbdrvce.h>
#include <srldrvce.h>
#include <stdbool.h>
#include "../devices.h"

bool serial_read_to_size(size_t size);
void serial_write(void *buf, size_t size);


srl_device_t srl;
uint8_t *srl_buf = net_device_buffer;

net_mode_t mode_srl = {
        MODE_SERIAL,
        usb_process,
        serial_read_to_size,
        serial_write
};

bool serial_read_to_size(size_t size){
    static size_t bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_parse_buffer[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void serial_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}

static usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data) {
                                    
    usb_error_t err;
    
    /* Delegate to srl USB callback */
    if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS)
        return err;
	
    /* Enable newly connected devices */
	switch(event){
		case USB_DEVICE_CONNECTED_EVENT:
			if (!(usb_GetRole() & USB_ROLE_DEVICE)){
				usb_device_t usb_device = event_data;
				usb_ResetDevice(usb_device);
			}
			break;
		case USB_HOST_CONFIGURE_EVENT:
			usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
			if(host) usb_device = host;
			goto srl_setup;
		case USB_DEVICE_ENABLED_EVENT:
			usb_device = event_data;
		srl_setup:
			if(!srl_Open(&srl, device, srl_buf, NET_BUFFER_SIZE, SRL_INTERFACE_ANY, 115200)){
				inet_flags |= 0b00000001;
				return USB_SUCCESS;
			}
			break;
		case USB_DEVICE_DISCONNECTED_EVENT:
			srl_Close(&srl);
			inet_flags = 0;
			device = NULL;
			break;
				
	}
    return USB_SUCCESS;
}
