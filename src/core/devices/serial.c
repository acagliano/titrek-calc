
#include <libload.h>
#include <srldrvce.h>
#include <stdbool.h>
#include "network.h"

srl_device_t srl;
uint8_t *srl_buf = net_device_buffer;

net_mode_t mode_srl = {
        MODE_SERIAL,
        &init_usb,
        &usb_process,
        &srl_read_to_size,
        &srl_write
};

bool serial_init(void){
    
    srl_error_t srl_error;
    if(!libload_IsLibLoaded(SRLDRVCE)) return false;
    
    // initialize usb device
    init_usb();     // perhaps PASS the descriptors for the serial device to init_usb???
    
    srl_error = srl_Open(&srl, device, srl_buf, NTWK_BUFFER_SIZE, SRL_INTERFACE_ANY, 115200);
    if(srl_error) return false;
    
    mode = &mode_srl;
    
    return true;
    
}


bool srl_read_to_size(size_t size){
    static size_t bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_buf[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void srl_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}
