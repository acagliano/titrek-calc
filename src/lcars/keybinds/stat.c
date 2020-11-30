
#include "keyfuncs.h"

void keybind_Stat(void){
    if((screen == SCRN_MAINS) || (screen == SCRN_TACT)){
        uint8_t slot = (screen == SCRN_MAINS) ? select.mains : select.tactical;
        ntwk_send(MODULE_INFO_REQUEST, PS_VAL(slot));
        screen = setbits(screen, SCRN_INFO);
    }
}
