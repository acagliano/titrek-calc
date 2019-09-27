// Module Handlers
#include "modules.h"
#include "../gfx/trekgui.h"

moduleindex_t moduledb[TARG_SENS] = {0};
#define MODICON_START 14
#define MODICON_END sizeof(trekgui)/sizeof(uint8_t*)

void init_CreateModuleDB(void){
    uint24_t i;
    for(i = MODICON_START; i < MODICON_END; i++){
        
    }
}
