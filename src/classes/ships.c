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

/*int module_GetEffectiveness(module_t* module, char steps){
    int effective = module->power.usage * module->health.current * 100 / module->power.baseusage / module->health.max;
    effective = ceil(effective * steps / 100);
    return effective;
}
*/
