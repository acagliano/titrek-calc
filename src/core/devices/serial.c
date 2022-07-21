
#include <libload.h>
#include <srldrvce.h>
#include <stdbool.h>
#include <fontlibc.h>
#include <graphx.h>
#include "../network.h"
#include "../gameloop.h"

static usb_error_t srl_handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data);
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

#define DEVICE_INIT_TIMEOUT_SECONDS 5
#define DEVICE_INIT_TIMEOUT_TPS 175000
#define DEVICE_INIT_TIMEOUT (DEVICE_INIT_TIMEOUT_TPS * DEVICE_INIT_TIMEOUT_SECONDS)
bool serial_init(void){
    
    if(!libload_IsLibLoaded(SRLDRVCE)) return false;
    
    fontlib_DrawString("init serial device... ");
    gfx_BlitBuffer();
    // initialize usb device
    init_usb(srl_GetCDCStandardDescriptors(), DEVICE_INIT_TIMEOUT, srl_handle_usb_event);
    
    mode = &mode_srl;
    return true;
    
}


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
    srl_error_t s_err;
    
    /* Delegate to srl USB callback */
    if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS)
        return err;
    /* Enable newly connected devices */
    if(event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE)) {
        usb_device_t device = event_data;
        usb_ResetDevice(device);
    }
    if(event == USB_HOST_CONFIGURE_EVENT) {
        usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
        if(host) device = host;
        s_err = srl_Open(&srl, device, srl_buf, NTWK_BUFFER_SIZE, SRL_INTERFACE_ANY, 115200);
        if(!s_err) net_device = MODE_SERIAL;
    }
    /* When a device is connected, or when connected to a computer */
    if((event == USB_DEVICE_ENABLED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))) {
        device = event_data;
        s_err = srl_Open(&srl, device, srl_buf, NTWK_BUFFER_SIZE, SRL_INTERFACE_ANY, 115200);
        if(!s_err) net_device = MODE_SERIAL;
    }
    if(event == USB_DEVICE_DISCONNECTED_EVENT) {
        srl_Close(&srl);
        tick_loop_mode = NO_CONNECTION;
        net_device = MODE_NTWK_NONE;
        device = NULL;
    }
    
    return USB_SUCCESS;
}
