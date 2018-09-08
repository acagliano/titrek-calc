
#ifndef processes_h
#define processes_h
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"

//void PROC_PowerDraw(Module_t *ShipModules, char repairing);
void proc_PowerCycle(Module_t *ShipModules, char limit, char repairing, char speed, Player_t *player, bool powerwarp, Module_t *warpcore, Module_t *auxpower);
void proc_DrawPower(Module_t *module);
void proc_ConditionalDraw(void);
void proc_MoveEntity(Position_t *pos, char speed, bool tickodd);

void Module_NoPower(Module_t* module);


#endif
