#include <stddef.h>
#include "datatypes/shipmodules.h"

enum pointertypes {
    shield,
    integrity,
    auxiliary
};

Module_t* init_SetPointer(Module_t *ShipModules, char type){
    // use *shields to update the ptr
    char i;
    Module_t* response = NULL;
    for(i = 0; i <= 20; i++){
        Module_t* module = &ShipModules[i];
        switch(type){
            case shield:
                if(module->modtype == mt_shield && module->online) response = &ShipModules[i];
                break;
            case integrity:
                if(module->modtype == mt_system && module->techtype == tt_integrity && module->online)
                    response = &ShipModules[i];
                break;
            case auxiliary:
                if(module->modtype == mt_system && module->techtype == tt_auxiliary && module->online)
                    response = &ShipModules[i];
                break;
        }
    }
    return response;
}
