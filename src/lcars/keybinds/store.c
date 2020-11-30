
#include "keyfuncs.h"

void keybind_Store(void){
    switch(screen){
        case SCRN_NAVIG:
        {
            uint8_t engine = select.engine;
            ntwk_send(ENGINE_SETSPEED, PS_VAL(engine), PS_VAL(select.newspeed));
        }
    }
}
