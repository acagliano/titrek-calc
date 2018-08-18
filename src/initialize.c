#include <stddef.h>
#include "datatypes/shipmodules.h"

Module_t* init_SetPointer(Module_t *ShipModules, char type, char start){
    // use *shields to update the ptr
    char i;
    Module_t* response = NULL;
    for(i = start; i <= 20; i++){
        Module_t* module = &ShipModules[i];
        if(module->techtype == type) response = &ShipModules[i];
    }
    return response;
}
