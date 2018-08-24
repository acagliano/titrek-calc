#include <stddef.h>
#include "datatypes/shipmodules.h"

Module_t* init_SetPointer(Module_t *ShipModules, char limit, char type, char start){
    // use *shields to update the ptr
    char i;
    Module_t* response = NULL;
    for(i = start; i < limit; i++){
        Module_t* module = &ShipModules[i];
        if(module->techtype == type) response = &ShipModules[i];
    }
    return response;
}
