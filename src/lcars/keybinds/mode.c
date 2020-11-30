
#include "keyfuncs.h"

void keybind_Mode(void){
    if((screen == SCRN_MAINS) || (screen == SCRN_TACT)){
        uint8_t slot = (screen == SCRN_MAINS) ? select.mains : select.tactical;
        const uint8_t action = CHANGE_ONLINE_STATE;
        ntwk_send(MODULE_STATE_CHANGE, PS_VAL(slot), PS_VAL(action));
    }
}
