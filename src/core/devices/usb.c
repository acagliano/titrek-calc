
#include <tice.h>
#include <usbdrvce.h>
#include "../../asm/exposure.h"
#include "network.h"

usb_device_t device;
static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data);

bool init_usb(void) {
    usb_error_t usb_error;
    sk_key_t key = 0;
    usb_error = usb_Init(handle_usb_event, NULL, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS);
    do {
        usb_HandleEvents();
        key = os_GetCSC();
    } while((!device) && (key!= sk_Clear));
    if(!device) return false;
    return true;
}

/* Handle USB events */

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data) {
    usb_error_t err;
    /* Delegate to srl USB callback */
    if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS)
        return err;
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

/*
bool usb_read_to_size(size_t size){
    static size_t bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_buf[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void usb_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}
*/

void usb_process(void) {
    usb_HandleEvents();
}
