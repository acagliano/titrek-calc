#ifndef gamestate_h
#define gamestate_h

#include <stdint.h>
#include <stdbool.h>
#include <tice.h>

#include "inet/inet.h"
#include "iface/ships.h"


struct _cli_version { uint8_t maj; uint8_t min; uint24_t build; };

#define VMAJOR	0
#define VMINOR	1
#define VBUILD	113

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#ifdef VBUILD
#define VSTRING	"v" TOSTRING(VMAJOR) "." TOSTRING(VMINOR) " (" TOSTRING(VBUILD) ")"
#else
#define VSTRING	"v" TOSTRING(VMAJOR) "." TOSTRING(VMINOR)
#endif


enum _gameflags {
	FRAME_DIRTY 		= 0,
	ENABLE_SECURE_INET	= 22,
	EV_LISTENER_ACTV	= 23
};

enum _screens {
	SCRN_NONE,
	SCRN_SPLASH,
	SCRN_SERVERLIST,
	SCRN_ABOUT,
	SCRN_CONSOLE,
	SCRNS_TOTAL
};

#define SET_FLAG(flagLoc, flagName)	((flagLoc) |= (1<<(flagName)))
#define RESET_FLAG(flagLoc, flagName) ((flagLoc) &= ~(1<<(flagName)))
#define GET_FLAG(flagLoc, flagName) ((flagLoc) & (1<<(flagName)))

#define MARK_FRAME_DIRTY	SET_FLAG(gamestate.gameflags, FRAME_DIRTY)
#define MARK_FRAME_CLEAN	RESET_FLAG(gamestate.gameflags, FRAME_DIRTY)

#define MAX_SERVER_COUNT	20

struct screendata_t {uint8_t selected; uint8_t num_opts;};
typedef struct _gamestate_t {
	uint24_t gameflags;
	uint24_t screen_up;
	struct screendata_t screendata[SCRNS_TOTAL];
	sk_key_t key;
	char server_identities[MAX_SERVER_COUNT][9];
	inetcfg_t inet;
	shipdata_t ship;
} gamestate_t;
extern gamestate_t gamestate;

enum _exitcodes {
	EXIT_OK,
	ERR_CRYPTOGRAPHY,
	ERR_USBDRV_NOT_FOUND,
	ERR_USBDRV_ERROR,
	ERR_IFACE_NOT_FOUND,
	ERR_ALLOCATION
};

#endif
