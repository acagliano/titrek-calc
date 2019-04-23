#include <stddef.h>
#include "ships.h"

// POWER FUNCTIONS
signed int power_GetBatteryPercent(power_t* power){
    return power->current * 100 / power->capacity;
}

signed int power_GetSpendPercent(power_t* power){
    return power->spend * 100 / power->base;
}

void power_SetDrawSource(power_t* power, char source){
    power->drawFrom = source;
}

char power_GetDrawSource(power_t* power){
    return power->drawFrom;
}

signed int power_GetPowerSpend(power_t* power){
    return power->spend;
}

void power_ChangeSpend(power_t* power, char amount){
    if(amount == POWER_INC)
        if(power->spend < (2 * power->base)) power->spend++;
    if(amount == POWER_DEC)
        if(power->spend > 0) power->spend--;
}

signed int power_GetPowerDraw(power_t* power){
    return power->base;
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
