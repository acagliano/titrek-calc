
#include <tice.h>
#include <usbdrvce.h>
#include "../../asm/exposure.h"
#include "../network.h"

usb_device_t device;

bool init_usb(usb_standard_descriptors_t* descriptors,
            usb_error_t (*handle_usb_event)(usb_event_t event,
                                            void *event_data,
                                            usb_callback_data_t *callback_data)){
    usb_error_t usb_error;
    sk_key_t key = 0;
    usb_error = usb_Init(handle_usb_event, NULL, descriptors, USB_DEFAULT_INIT_FLAGS);
    do {
        usb_HandleEvents();
        key = getKey();
    } while((!device) && (key!= sk_Clear));
    if(!device) return false;
    return true;
}


void usb_process(void) {
    usb_HandleEvents();
}
