#ifndef gamestate_h
#define gamestate_h

#include <stdint.h>
#include <stdbool.h>

#include "inet/devices.h"

typedef struct _gamestate_t {
	struct inet_data_t inet_data;
} gamestate_t;

enum _exitcodes {
	EXIT_OK,
	ERR_CRYPTOGRAPHY,
	ERR_USBDRV_NOT_FOUND,
	ERR_USBDRV_ERROR,
	ERR_IFACE_NOT_FOUND,
};

#endif
