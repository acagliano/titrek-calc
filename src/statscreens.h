#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20

extern char mainsys_strings[][10];

void Screen_RenderUI(uint24_t screen, ship_t* modules, selected_t* select);
void Screen_UISystemStats(module_t* systems, uint24_t selected);
void Screen_UITacticalStats(module_t* tactical, uint24_t selected);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);

int Stats_DrawHealthBar(unsigned int percent, unsigned int length, int x, int y, unsigned char border_color, unsigned char bar_color, gfx_rletsprite_t* icon);
void Screen_Background(unsigned char active);


#endif
