
#ifndef processes_h
#define processes_h
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"

void PROC_PowerDraw(Module_t *ShipModules, char repairing);
void PROC_PowerCycle(Module_t *ShipModules, Module_t *warpcore, Module_t *auxpower, char repairing, Player_t *player, bool powerwarp);
void proc_MoveEntity(Position_t *pos, char speed, bool tickodd);

void Module_NoPower(Module_t* module);


#endif
