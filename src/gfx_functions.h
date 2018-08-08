
#ifndef gfx_functions_h
#define gfx_functions_h
#include <stdbool.h>
#include "datatypes/mapdata.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"

void lcars_DrawHealthBar(int percent, char scale, int x, int y, bool text);
void gfx_WipeScreen(void);
void PrintHeader(char *text, char x, char y, char xtextOff, char ytextOff);
void DrawGUI();
void ResetXY(void);
void gfx_DrawShipStatusIcon(Module_t* integrity, Module_t* shields);
void gfx_DrawInventoryStatusIcon(bool status);
void gfx_DrawCoreBreachAlert(bool status);
void gfx_DrawLifeSupportAlert(bool status);
void gfx_DrawSpeedIndicator(char speed, char maxspeed);
void GUI_ViewScreen(MapData_t *map, Position_t *playerpos);

#endif
