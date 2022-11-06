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
#include <tice.h>

#include <libload.h>
#include <graphx.h>

#include "inet/devices.h"
#include "io/keydetect.h"
#include "io/frames.h"
#include "gamestate.h"

#include "ev.h"

#define LOAD_CRYPTX_LIBS	\
		(libload_IsLibLoaded(HASHLIB) && libload_IsLibLoaded(ENCRYPT))

gamestate_t gamestate = {0};

int main(void) {

    // init the program
	gamestate.gameflags |= (1<<EV_LISTENER_ACTV);
	atexit(usb_Cleanup);
	atexit(gfx_End);
	//atexit(prgm_CleanUp);
	
	// initialize cryptographic libraries, disable encryption if not found
	gamestate.inet_data.inet_flags |= (LOAD_CRYPTX_LIBS<<INET_ENABLE_ENCRYPTION);
	
	gfx_Begin();
	gfx_SetDrawBuffer();
	ntwk_init();
	srandom(rtc_Time());
	gamestate.screen_up = SCRN_SPLASH;
	enqueue(io_keydetect, true);
	enqueue(frame_render, true);
	
	while((gamestate.gameflags>>EV_LISTENER_ACTV) & 1){
		for(;queue_start != queue_stop; queue_start++){
			listener_t *runner = &listener_queue[queue_start];
			runner->exec();
			if(runner->requeue) enqueue(runner->exec, runner->requeue);
		}
	}
	exit(0);
}
