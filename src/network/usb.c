#include <usbdrvce.h>
#include <srldrvce.h>
#include <debug.h>
#include "../equates.h"
#include "network.h"

srl_device_t srl;
uint8_t srl_buf[8192];

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data);

bool init_usb(void) {
    usb_error_t usb_error;
    netflags.network_up = false;
    usb_error = usb_Init(handle_usb_event, NULL, NULL, USB_DEFAULT_INIT_FLAGS);
    return !usb_error;
}

/* Handle USB events */
static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data) {
    /* When a device is connected, or when connected to a computer */
    if((event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE)) || event == USB_HOST_CONFIGURE_EVENT) {
        usb_device_t device = event_data;
        char dummy;
        srl_error_t srl_error = srl_Init(&srl, device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY);
        if(!srl_error) srl_SetRate(&srl, 115200);
        if(!srl_error) srl_Read(&srl, &dummy, 1);
        if(!srl_error) {
            netflags.network_up = true;
        }
    }

    /* When a device is disconnected */
    if(event == USB_DEVICE_DISCONNECTED_EVENT) {
        netflags.network_up = false;
        netflags.logged_in = false;
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
