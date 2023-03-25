//--------------------------------------
// Program Name: Project TI-Trek
// Author: Anthony Cagliano
// License: GPL3.0
// Description:
//--------------------------------------

/* Keep these headers */
/* Standard headers - it's recommended to leave them included */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <tice.h>

#include <libload.h>
#include <graphx.h>
#include <encrypt.h>

#include "inet/inet.h"
#include "iface/keydetect.h"
#include "iface/frames.h"
#include "iface/fonts.h"
#include "gamestate.h"

#include "ev.h"

#define LOAD_CRYPTX_LIBS	\
		(libload_IsLibLoaded(HASHLIB) && libload_IsLibLoaded(ENCRYPT))

gamestate_t gamestate = {0};
#ifdef VBUILD
	struct _cli_version cli_version = { VMAJOR, VMINOR, VBUILD };
#else
	struct _cli_version cli_version = { VMAJOR, VMINOR, 0 };
#endif
#define CEMU_CONSOLE ((char*)0xFB0000)
int main(void) {

    // init the program
	gamestate.gameflags |= (1<<EV_LISTENER_ACTV);
	
	// set actions to take at program quit
	atexit(usb_Cleanup);
	atexit(gfx_End);
	//atexit(prgm_CleanUp);
	
	// initialize cryptographic libraries, exit with error if fail
	// init RNG, exit with error if fail
	if(!LOAD_CRYPTX_LIBS) exit(ERR_CRYPTOGRAPHY);
	if(!cryptx_csrand_init(SAMPLING_THOROUGH)) exit(ERR_CRYPTOGRAPHY);
	
	// set up graphics
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetTextTransparentColor(1);
	gfx_SetTextBGColor(1);
	gfx_SetFontData(font_haxor8);
	gfx_SetMonospaceFont(7);
	
	// internet up
	inet_init();
	srandom(rtc_Time());
	frame_screen_up(SCRN_SPLASH);
	enqueue(screendata_init, PROC_RENDER, false);
	enqueue(io_keydetect_menu, PROC_KEYDETECT, true);
	MARK_FRAME_DIRTY;
	
	while((gamestate.gameflags>>EV_LISTENER_ACTV) & 1){
		for(;queue_start != queue_stop; queue_start++){
			listener_t *runner = &listener_queue[queue_start];
			if(runner->proc_id){
				runner->exec();
				if(runner->requeue) enqueue(runner->exec, runner->proc_id, runner->requeue);
			}
		}
	}
	exit(0);
}
