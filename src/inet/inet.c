
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libload.h>
#include <usbdrvce.h>
#include <encrypt.h>
#include <hashlib.h>
#include "inet.h"
#include "conn.h"
#include "devices.h"
#include "../asm/exposure.h"
#include "../io/frames.h"
#include "../gamestate.h"
#include "../ev.h"

void inet_process_packet(uint8_t *data, size_t len);

#define CEMU_CONSOLE ((char*)0xFB0000)
void inet_init(void){
	
	if (!libload_IsLibLoaded(USBDRVCE)) exit(ERR_USBDRV_NOT_FOUND);
	
	if(libload_IsLibLoaded(TCPDRVCE)) return;	// unimplemented
	else if(libload_IsLibLoaded(SRLDRVCE)) {
		gamestate.inet.process = usb_HandleEvents;
		if(usb_Init(srl_handle_usb_event, NULL,
					srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS))
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

#define AES_SECRET_LEN	32
#define HMAC_SECRET_LEN	16
uint8_t secrets[AES_SECRET_LEN + HMAC_SECRET_LEN];
void inet_process_packet(uint8_t *data, size_t len){
	uint8_t ctl = *data;
	uint8_t *pkt_content = &data[1];
	if(ctl==PING_BRIDGE) {
		SET_FLAG(gamestate.inet.flags, INET_BRIDGE_UP);
		console_insert_line("bridge up, contacting server.", MSG_NORMAL);
		bridge->connect_to_server();
	}
	else if(GET_FLAG(gamestate.inet.flags, INET_BRIDGE_UP)){
		if(ctl==CONNECT){
			if(!(*pkt_content)){
				console_insert_line("connected to server.", MSG_NORMAL);
				console_insert_line("waiting for rsa.", MSG_NORMAL);
				SET_FLAG(gamestate.inet.flags, INET_CONNECTED);
			} else {
				console_insert_line("server connect failed.", MSG_ERROR);
				RESET_FLAG(gamestate.inet.flags, INET_BRIDGE_UP);
				frame_screen_up(SCRN_SERVERLIST);
			}
		}
		else if(GET_FLAG(gamestate.inet.flags, INET_CONNECTED)){
			if(ctl==DISCONNECT){
				RESET_FLAG(gamestate.inet.flags, INET_BRIDGE_UP);
				RESET_FLAG(gamestate.inet.flags, INET_CONNECTED);
				frame_screen_up(SCRN_SPLASH);
			} else if(ctl==RSA_SETUP){
				size_t rsa_len = len - 1;
				uint8_t e_buf[rsa_len];
				console_insert_line("generating secrets.", MSG_NORMAL);
				// generate secrets
				csrand_fill(secrets, sizeof secrets);
				console_insert_line("rsa encrypting secrets.", MSG_NORMAL);
				// encrypt secrets
				rsa_encrypt(secrets, sizeof secrets, e_buf, pkt_content, rsa_len, SHA256);
				console_insert_line("sending secrets.", MSG_NORMAL);
				inet_send_packet(1, AES_SECRET_ACK, e_buf, rsa_len);
				
			} else if(ctl==AES_SECRET_ACK){
				console_insert_line("encrypting user token.", MSG_NORMAL);
				size_t username_len = strlen(username);
				uint8_t pt[TOKEN_LEN + username_len];
				uint8_t ct[TOKEN_LEN + username_len];
				uint8_t iv[AES_IVSIZE];
				uint8_t hmac_digest[SHA256_DIGEST_LEN];
				hmac_ctx hmac;
				aes_ctx aes;
					
				// copy token and username into plaintext buffer
				memcpy(pt, token, TOKEN_LEN);
				memcpy(&pt[TOKEN_LEN], username, username_len);
				
				// generate iv, intialize AES context and encrypt
				csrand_fill(iv, sizeof iv);
				aes_init(&aes, secrets, AES_SECRET_LEN, iv, AES_MODE_CTR);
				aes_encrypt(&aes, pt, sizeof pt, ct);
				
				// generate authentication tag for data
				hmac_init(&hmac, &secrets[AES_SECRET_LEN], HMAC_SECRET_LEN, SHA256);
				hmac_update(&hmac, iv, sizeof iv);
				hmac_update(&hmac, ct, sizeof ct);
				hmac_final(&hmac, hmac_digest);
				
				// send iv + encrypted data + auth tag
				console_insert_line("sending token.", MSG_NORMAL);
				inet_send_packet(3, LOGIN,
								 iv, sizeof iv,
								 ct, sizeof ct,
								 hmac_digest, sizeof hmac_digest);
				
				// aes-encrypt token, send to server
			}
			else if(ctl==LOGIN){
				// result of login to server
				if(!(*pkt_content)){
					SET_FLAG(gamestate.inet.flags, INET_LOGGED_IN);
					inet_send_packet(0, LOAD_SHIP);
				}
				else {
					console_insert_line("login failed.", MSG_ERROR);
					RESET_FLAG(gamestate.inet.flags, INET_CONNECTED);
					RESET_FLAG(gamestate.inet.flags, INET_BRIDGE_UP);
					frame_screen_up(SCRN_SERVERLIST);
				}
			}
			else if(GET_FLAG(gamestate.inet.flags, INET_LOGGED_IN)){
				if(ctl==LOAD_SHIP){
					
				}
			}
		}
	}
}
