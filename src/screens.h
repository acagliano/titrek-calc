
#ifndef screens_h
#define screens_h
#include "datatypes/shipmodules.h"

void GUI_StatusReport(Module_t *ShipModules, char selected, char repairing);
void GUI_PowerReport(Module_t *ShipModules, char selected, char speed);
void GUI_TacticalReport(Module_t *ShipModules, Module_t* shields, Module_t* activeweap);
void GUI_InventoryReport(void);


#endif
