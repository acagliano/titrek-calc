
#ifndef screens_h
#define screens_h
#include "datatypes/shipmodules.h"
#include "datatypes/mapdata.h"
#include <stdbool.h>

void GUI_StatusReport(Module_t *ShipModules, char limit, char selected, char repairing);
void GUI_PowerReport(Module_t *ShipModules, char limit, char selected, bool icons_initialized, bool sourcewarp);
void GUI_TacticalReport(Module_t *ShipModules, char loops, Module_t* shields, Module_t* activeweap, bool icons_initialized);
void GUI_SensorReadout(MapData_t *map, unsigned int map_size, Player_t *player, Module_t *sensors);
void GUI_InventoryReport(void);


#endif
