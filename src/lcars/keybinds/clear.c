
#include "keyfuncs.h"

void keybind_Clear(void){
    if(netflags.logged_in){
        if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
        else ntwk_send_nodata(DISCONNECT);
    }
    else gameflags.exit = true;
}
