
#include <tice.h>
#include <stdio.h>
#include <usbdrvce.h>
#include "../../asm/exposure.h"
#include "../network.h"


usb_error_t timeout_proc(usb_timer_t *timer);


usb_error_t timeout_proc(usb_timer_t *timer){
    return USB_ERROR_TIMEOUT;
}
