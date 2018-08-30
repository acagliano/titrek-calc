
#ifndef processes_h
#define processes_h


void PROC_PowerDraw(Module_t *ShipModules, char repairing);
void PROC_PowerCycle(Module_t *ShipModules, Module_t *warpcore, Module_t *auxpower, char repairing, Player_t *player, bool powerwarp);
void proc_MoveShip(Position_t *pos, char speed);

void Module_NoPower(Module_t* module);


#endif
