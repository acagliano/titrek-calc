#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#include "../classes/screens.h"
#include "../classes/ships.h"
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20

typedef struct {
    uint24_t x, width;
    uint8_t y, height;
} window_data_t;

extern char mainsys_strings[][10];

int num_GetLength(int number);
void Screen_RenderUI(void);
void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected);
void Screen_UITacticalStats(hull_t hull, module_t* systems, uint24_t syscount, uint24_t selected);
void LCARS_RenderModuleList(module_t* systems, uint24_t syscount, uint8_t class, window_data_t* w, uint24_t selected);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);
void Overlay_UIModuleInfo(void);

int Stats_DrawHealthBar(unsigned int percent, unsigned int length, int x, int y, unsigned char border_color, unsigned char bar_color, gfx_rletsprite_t* icon);
void Screen_Background(unsigned char active);
void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y, uint24_t width);


#endif
