
#ifndef screens_h
#define screens_h
#include "datatypes/shipmodules.h"

void GUI_StatusReport(Module_t *ShipModules);
void GUI_PowerReport(Module_t *ShipModules);
void GUI_TacticalReport(Module_t *ShipModules, Module_t* shields);
void GUI_InventoryReport(void);


#endif
