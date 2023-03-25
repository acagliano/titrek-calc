#ifndef inet_h
#define inet_h

#include <stdint.h>
#include <stdarg.h>
#include "devices.h"

// inet_flags is a series of bits that indicate network status/config (see inet_flags)
#define INET_ACTIVE		0
#define INET_BRIDGE_UP	1
#define INET_CONNECTED	2
#define INET_LOGGED_IN	3
enum _inet_devices {
	DEVICE_TCP,
	DEVICE_SRL
};
typedef struct _inetcfg_t {
	uint8_t flags;
	uint8_t device_id;
	void *device;
	void (*process)();
	size_t (*recv)();
	size_t (*send)();
	void (*setdown)();
	uint32_t timeout;
} inetcfg_t;

enum _packetids {
	// CONNECTION/ADMIN
	// 0 - 9
	PING_BRIDGE,		// 0
	CONNECT,            // 1
	DISCONNECT,         // 2
	RSA_SETUP,     		// 3
	AES_SECRET_ACK,   	// 4
	LOGIN,            	// 5
	// This is for the client not up to date response , See response codes. VERSION_* must be the second byte of packet
	
	// Ship Interfacing: 20 - 29
	LOAD_SHIP = 0x10,
	MODULE_INFO_REQUEST,            // 21
	MODULE_CONFIG,            // 22
	NEW_GAME_REQUEST,               // 23
	GET_ENGINE_MAXIMUMS,            // 24
	ENGINE_SETSPEED,                // 25
	
	// GAMEPLAY
	// Map Interfacing: 10 - 19
	FRAMEDATA_REQUEST = 0x20,
	SENSOR_DATA_REQUEST,        // 11
	PLAYER_MOVE,                // 12
	POSITION_REQUEST,           // 13
	
	// Map Streaming : 30 - 39
	RENDER_FBUF_OBJ = 30,
	RENDER_FBUF_DATA,
	
	// File Streaming
	GFXCACHE_INIT	= 0xe0,
	GFXCACHE_LOAD	= 0xe1,
	GFXCACHE_DONE	= 0xe2,
	
	// DEBUG: 0xf*
	BRIDGE_ERROR = 0xf0,
	PING_SERVER = 0xf1,
	MESSAGE,
	DEBUG,
	SERVINFO
};


void inet_init(void);
size_t inet_send_packet(int ps_count, uint8_t ctl, ...);
void inet_get_packet(void);

#endif
