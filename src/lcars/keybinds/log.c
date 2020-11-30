
#include "keyfuncs.h"

void keybind_Log(void){
    if(screen==SCRN_NAVIG) screen=SCRN_OFF;
    else {
        ntwk_send_nodata(GET_ENGINE_MAXIMUMS);
        screen=SCRN_NAVIG;
    }
}
