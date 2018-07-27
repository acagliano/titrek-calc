
#ifndef gfx_functions_h
#define gfx_functions_h
#include <stdbool.h>
#include "datatypes/shipmodules.h"

void lcars_DrawHealthBar(int percent, char scale, int x, int y, bool text);
void gfx_WipeScreen(void);
void PrintHeader(char *text, char x, char y, char xtextOff, char ytextOff);
void DrawGUI();
void GUI_UpdateIcons(Module_t* ShipModules);
void ResetXY(void);
void gfx_DrawShipStatusIcon(bool status);
void gfx_DrawShieldStatusIcon(bool status);
void gfx_DrawPowerStatusIcon(bool status);
void gfx_DrawInventoryStatusIcon(bool status);

#endif
