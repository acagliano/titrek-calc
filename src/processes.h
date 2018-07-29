
#ifndef processes_h
#define processes_h


void PROC_PowerDraw(Module_t *ShipModules, char repairing);
void PROC_PowerCycle(Module_t *ShipModules, char repairing);

void Module_NoPower(Module_t* module);


#endif
