
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <libload.h>
#include <usbdrvce.h>
#include "inet.h"
#include "../gamestate.h"
#include "../ev.h"

void inet_process_packet(void *data, size_t len);

void inet_init(void){
	uint8_t inet_devices = 0;
	
	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	inet_devices |= libload_IsLibLoaded(TCPDRVCE);
	inet_devices |= (libload_IsLibLoaded(SRLDRVCE)<<1);
	if (!inet_devices) exit(ERR_IFACE_NOT_FOUND);
	
	gamestate.inet.process = usb_HandleEvents;
	
	for(uint8_t i=0; i<INET_DEVICECT;i++){
		if ((inet_devices>>i) & 1) {
			if (usb_Init(device_idata[i].usb_handler,
						 NULL,
						 device_idata[i].descriptors,
						 USB_DEFAULT_INIT_FLAGS))
				exit(ERR_USBDRV_ERROR);
			enqueue(gamestate.inet.process, PROC_NTWK, true);
			return;
		}
	}
}



#define INET_OUTBUF_SIZE	2048
uint8_t inet_outbuf[INET_OUTBUF_SIZE];
size_t inet_send_packet(int ps_count, uint8_t ctl, ...){
	inetcfg_t *inet = &gamestate.inet;
	size_t total_len = 1, arg_len;
	va_list vptr;
	void *arg = NULL;
	
	// copy control code
	inet_outbuf[0] = ctl;
	
	va_start(vptr, ctl);
	for(int i = 0; i < ps_count; i++){
		arg = va_arg(vptr, void*);
		arg_len = va_arg(vptr, int);
		if ((total_len + arg_len) > INET_OUTBUF_SIZE) break;
		memcpy(&inet_outbuf[total_len], arg, arg_len);
		total_len += arg_len;
	}
	va_end(vptr);
	
	inet->send(inet->device, &total_len, sizeof total_len);
	inet->send(inet->device, inet_outbuf, total_len);
	return total_len;
 }

#define INET_INBUF_SIZE	2048
uint8_t inet_inbuf[INET_INBUF_SIZE];

bool inet_read_to_size(size_t size){
	inetcfg_t *inet = &gamestate.inet;
	static size_t bytes_read = 0;
	bytes_read += inet->recv(inet->device, &inet_inbuf[bytes_read], size - bytes_read);
	if(bytes_read >= size) {bytes_read = 0; return true;}
	else return false;
}

#define INET_TIMEOUT_SEC	60
void inet_get_packet(void){
	static size_t packet_size = 0;
	if(packet_size){
		if(inet_read_to_size(packet_size)){
			inet_process_packet(inet_inbuf, packet_size);
			packet_size = 0;
		}
	} else if(inet_read_to_size(sizeof packet_size)) packet_size = *(size_t*)inet_inbuf;
	else gamestate.inet.timeout++;
}


void inet_process_packet(void *data, size_t len){
	
}
