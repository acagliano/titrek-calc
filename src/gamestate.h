#ifndef gamestate_h
#define gamestate_h

#include <stdint.h>
#include <stdbool.h>
#include <tice.h>

#include "inet/devices.h"


struct _cli_version { uint8_t maj; uint8_t min; uint24_t build; };

#define VMAJOR	0
#define VMINOR	1
#define VBUILD	112

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#ifdef VBUILD
#define VSTRING	"v" TOSTRING(VMAJOR) "." TOSTRING(VMINOR) " (" TOSTRING(VBUILD) ")"
#else
#define VSTRING	"v" TOSTRING(VMAJOR) "." TOSTRING(VMINOR)
#endif


enum _gameflags {
	FRAME_DIRTY 		= 0,
	EV_LISTENER_ACTV	= 23
};
#define MARK_FRAME_DIRTY	gamestate.gameflags |= (1<<FRAME_DIRTY)

enum _screens {
	SCRN_NONE,
	SCRN_SPLASH,
	SCRN_SERVERLIST,
	SCRN_SETTINGS,
	SCRNS_TOTAL
};

#define MAX_SERVER_COUNT	10
struct screendata_t {uint8_t selected; uint8_t num_opts;};
typedef struct _gamestate_t {
	uint24_t gameflags;
	uint24_t screen_up;
	struct screendata_t screendata[SCRNS_TOTAL];
	sk_key_t key;
	char server_identities[MAX_SERVER_COUNT][9];
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
