
#include <tice.h>
#include <stdio.h>
#include <usbdrvce.h>
#include "../../asm/exposure.h"
#include "../network.h"

usb_device_t device;
usb_error_t timeout_proc(usb_timer_t *timer);

bool init_usb(usb_standard_descriptors_t* descriptors,
            netwait_timeout_t timeoutms,
            usb_error_t (*handle_usb_event)(usb_event_t event,
                                            void *event_data,
                                            usb_callback_data_t *callback_data)){
    usb_error_t usb_error;
    usb_timer_t timeout_config;
    timeout_config.handler = timeout_proc;
    sk_key_t key = 0;
    usb_error = usb_Init(handle_usb_event, NULL, descriptors, USB_DEFAULT_INIT_FLAGS);
    usb_StartTimerMs(&timeout_config, timeoutms);
    while(1){
        if(device) break;
        if(usb_HandleEvents() == USB_ERROR_TIMEOUT) break;
    }
    if(!device) return false;
    return true;
}


void usb_process(void) {
    usb_HandleEvents();
}

usb_error_t timeout_proc(usb_timer_t *timer){
    return USB_ERROR_TIMEOUT;
}
