#include <tice.h>
#include <usbdrvce.h>
#include <srldrvce.h>
#include <debug.h>
#include "../equates.h"
#include "network.h"

srl_device_t srl;
usb_device_t device;
uint8_t srl_buf[8192];

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data);

bool init_usb(void) {
    usb_error_t usb_error;
    srl_error_t srl_error;
    size_t usb_timeout = 500000;
    netflags.network_up = false;
    usb_error = usb_Init(handle_usb_event, NULL, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS);
    do {
        usb_HandleEvents();
        usb_timeout--;
    } while((!device) && usb_timeout);
    if(!device) {
        printf("no device");
        os_GetKey();
        return false;
    }
    srl_error = srl_Open(&srl, device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY, 115200);
    if(srl_error) {
        printf("srl_error");
        os_GetKey();
        return false;
    }
    netflags.network_up = true;
    return true;
}

/* Handle USB events */

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data __attribute__((unused))) {
    /* Enable newly connected devices */
    if(event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE)) {
        usb_device_t device = event_data;
        printf("device connected\n");
        usb_ResetDevice(device);
    }
    if(event == USB_HOST_CONFIGURE_EVENT) {
        usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
        if(host) device = host;
    }
    /* When a device is connected, or when connected to a computer */
    if((event == USB_DEVICE_ENABLED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))) {
        device = event_data;
    }
    if(event == USB_DEVICE_DISCONNECTED_EVENT) {
        srl_Close(&srl);
        netflags.network_up = false;
        device = NULL;
    }

    return USB_SUCCESS;
}

bool usb_read_to_size(size_t size){
    static bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_buf[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void usb_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}

void usb_process(void) {
    usb_HandleEvents();
}

net_mode_t mode_srl = {
        MODE_SERIAL,
        &init_usb,
        &usb_process,
        &usb_read_to_size,
        &usb_write
};
