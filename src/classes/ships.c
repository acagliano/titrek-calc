#include <stddef.h>
#include "ships.h"

// POWER FUNCTIONS
signed int power_GetBatteryPercent(power_t* power){
    return power->current * 100 / power->capacity;
}

signed int power_GetExpendPercent(power_t* power){
    return power->usage * 100 / power->baseusage;
}

void power_SetDraw(power_t* power, char source){
    power->drawFrom = source;
}


// HEALTH FUNCTIONS
signed int health_GetHealthPercent(health_t* health){
    return health->current * 100 / health->max;
}

void health_DamageModule(health_t* health, int amount){
    health->current += amount;
    if(health->current < 0) health->current = 0;
    if(health->current > health->max) health->current = health->max;
}


// MODULE FUNCTIONS
char module_GetOnlineState(module_t* module){
    return module->online;
}

char module_SetOnlineState(module_t* module, char state){
    if(state){
        if(module->power.current == 0) return SETSTATE_NOPOWER;
        if(state == 1 && module->health.current == 0) return SETSTATE_NOHEALTH;
    }
    module->online = state;
    return SETSTATE_SUCCESS;
}

void* module_GetDataPtr(module_t* module){
    unsigned char class = module->techclass;
    void* data;
    switch(class){
        case MC_SYSTEM:
            data = module_GetSysDataPtr(&module->moduleclass);
            break;
        
        case MC_TACTICAL:
            data = module_GetTactDataPtr(&module->moduleclass);
            break;
    }
    return data;
}

void* module_GetSysDataPtr(class_t* moduleclass){
    unsigned char type = moduleclass->techtype;
    switch(type){
        case M_INTEG:
            return (void*)&moduleclass->data.sys_data.integ_data;
        case M_LIFESUP:
            return (void*)&moduleclass->data.sys_data.lifesup_data;
        case M_CORE:
            return (void*)&moduleclass->data.sys_data.core_data;
        case M_WARPDR:
        case M_IMPDR:
            return (void*)&moduleclass->data.sys_data.engine_data;
        case M_NAVSENS:
            return (void*)&moduleclass->data.sys_data.navsens_data;
        case M_TRANS:
            return (void*)&moduleclass->data.sys_data.trans_data;
        default:
            return NULL;
    }
}

void* module_GetTactDataPtr(class_t* moduleclass){
    unsigned char type = moduleclass->techtype;
    switch(type){
        case M_TACTSENS:
            return (void*)&moduleclass->data.tact_data.tact_t.targsens_data;
        case M_SHIELD:
            return (void*)&moduleclass->data.tact_data.tact_t.shield_data;
        case M_PHASER:
        case M_TORPEDO:
            return (void*)&moduleclass->data.tact_data.tact_t.weapon_data;
        default:
            return NULL;
    }
}


void module_SetHealthMax(health_t* health){
    health->current = 50;
    health->max = 50;
}

void module_SetPowerMax(power_t* power){
    power->capacity = 100;
    power->current = 100;
}

/*int module_GetEffectiveness(module_t* module, char steps){
    int effective = module->power.usage * module->health.current * 100 / module->power.baseusage / module->health.max;
    effective = ceil(effective * steps / 100);
    return effective;
}
*/
