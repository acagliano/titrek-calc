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
            data = module_GetSysDataPtr(&module->data);
            break;
        
        case MC_TACTICAL:
            data = module_GetTactDataPtr(&module->data);
            break;
    }
    return data;
}

void* module_GetSysDataPtr(data_t* data){
    unsigned char type = data->techtype;
    switch(type){
        case M_INTEG:
            return (void*)&data->type.system.integrity;
            break;
        case M_LIFESUP:
            return (void*)&data->type.system.lifesupport;
            break;
        case M_CORE:
            return (void*)&data->type.system.core;
            break;
        case M_WARPDR:
        case M_IMPDR:
            return (void*)&data->type.system.engine;
            break;
        case M_NAVSENS:
            return (void*)&data->type.system.navsens;
            break;
        case M_TRANS:
            return (void*)&data->type.system.transport;
            break;
        default:
            return NULL;
    }
}

void* module_GetTactDataPtr(data_t* data){
    unsigned char type = data->techtype;
    switch(type){
        case M_TACTSENS:
            return (void*)&data->type.tactical.targsens;
            break;
        case M_SHIELD:
            return (void*)&data->type.tactical.shields;
            break;
        case M_PHASER:
        case M_TORPEDO:
            return (void*)&data->type.tactical.weapons;
            break;
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
