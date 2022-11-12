
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libload.h>
#include <usbdrvce.h>
#include "inet.h"
#include "devices.h"
#include "../asm/exposure.h"
#include "../gamestate.h"
#include "../ev.h"

void inet_process_packet(void *data, size_t len);

#define CEMU_CONSOLE ((char*)0xFB0000)
void inet_init(void){
	
	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	if(libload_IsLibLoaded(TCPDRVCE)) return;	// unimplemented
	else if(libload_IsLibLoaded(SRLDRVCE)) {
		gamestate.inet.process = usb_HandleEvents;
		if(usb_Init(srl_handle_usb_event, NULL,
					srl_GetCDCStandardDescriptors, USB_DEFAULT_INIT_FLAGS))
			exit(ERR_USBDRV_ERROR);
		enqueue(gamestate.inet.process, PROC_NTWK, true);
	}
	else exit(ERR_IFACE_NOT_FOUND);
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
