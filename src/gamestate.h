#ifndef gamestate_h
#define gamestate_h

#include <stdint.h>
#include <stdbool.h>

#include "inet/devices.h"

enum _gameflags {
	FRAME_DIRTY 		= 0,
	EV_LISTENER_ACTV	= 23
};

enum _screens {
	SCRN_NONE,
	SCRN_SPLASH,
	SCRN_SERVERLIST
}
typedef struct _gamestate_t {
	uint24_t gameflags;
	uint24_t screen_up;
	struct inet_data_t inet_data;
} gamestate_t;
extern gamestate_t gamestate;

enum _exitcodes {
	EXIT_OK,
	ERR_CRYPTOGRAPHY,
	ERR_USBDRV_NOT_FOUND,
	ERR_USBDRV_ERROR,
	ERR_IFACE_NOT_FOUND,
};

#endif
