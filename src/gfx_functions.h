
#ifndef gfx_functions_h
#define gfx_functions_h
#include <stdbool.h>
#include "datatypes/mapdata.h"
#include "datatypes/shipmodules.h"
#include "datatypes/playerdata.h"

#define SPARK_ANIM_DURA 15
typedef struct {
    unsigned int origin_x;
    unsigned char origin_y;
    char duration, maxduration;
} animation_t;

typedef struct {
    char type;
    long distance;
    char angleOffsets[2];
} RenderItem_t;

void lcars_DrawHealthBar(int percent, char scale, int x, int y, bool text);
void gfx_WipeScreen(void);
void PrintHeader(char *text, char y);
void DrawGUI();
void ResetXY(void);
void gfx_DrawShipStatusIcon(Module_t* integrity, Module_t* shields, Player_t* player);
void gfx_DrawInventoryStatusIcon(bool status);
void gfx_DrawCoreBreachAlert(void);
void gfx_DrawLifeSupportAlert(void);
void gfx_DrawSpeedIndicator(char speed, char maxspeed_warp, char maxspeed_impulse, bool icons_enabled);
void vfx_RenderSparkFlare(animation_t *animate);
void gfx_RenderOrientation(unsigned char anglexz, unsigned char angley, int dialx, unsigned char dialy);
//void GUI_ViewScreen(MapData_t *map, Position_t *playerpos);
extern const char *trek_version;

#endif
