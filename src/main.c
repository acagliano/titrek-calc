//--------------------------------------
// Program Name: Project TI-Trek
// Author: Anthony Cagliano
// License: GPL3.0
// Description:
//--------------------------------------

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <intce.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h, debug.h
#include <graphx.h>

#include "rendering/init.h"
#include "inet/devices.h"

#include "ev.h"

#define LOAD_CRYPTX_LIBS	\
		(libload_IsLibLoaded(HASHLIB) && libload_IsLibLoaded(ENCRYPT))

gamestate_t gamestate = {0};

void exit_cleanup(void){
	usb_CleanUp();
	gfx_End();
	prgm_CleanUp();
}


int main(void) {

    // init the program
	bool listener_active = true;
	atexit(exit_cleanup);
	
	// initialize cryptographic libraries, disable encryption if not found
	gamestate.inet_data.inet_flags |= (LOAD_CRYPTX_LIBS<<INET_ENABLE_ENCRYPTION);
	
	gfx_Begin();
	ntwk_init();
	
	while(listener_active){
		for(;queue_start != queue_stop; queue_start++){
			listener_t *runner = &listener_queue[queue_start];
			runner->exec(runner->data);
			if(runner->requeue) enqueue(runner->exec, runner->data, runner->requeue);
		}
	}
	exit(0);
}
