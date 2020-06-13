#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20

extern char mainsys_strings[][10];

void Screen_RenderUI(uint24_t screen, selected_t* select);
void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected);
void Screen_UITacticalStats(module_t* systems, uint24_t syscount, uint24_t selected);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);
void Overlay_UIModuleInfo(module_t* module);

int Stats_DrawHealthBar(unsigned int percent, unsigned int length, int x, int y, unsigned char border_color, unsigned char bar_color, gfx_rletsprite_t* icon);
void Screen_Background(unsigned char active);
void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y);


#endif
