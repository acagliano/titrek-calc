#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20

extern char mainsys_strings[][10];

void Screen_UIModuleStats(char *title, module_t* modules, unsigned char count);
void Screen_UIRepairSys(module_t* modules, unsigned char count);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);

void Stats_DrawHealthBar(unsigned char percent, unsigned int length, int x, int y);
void Screen_Background(unsigned char active);


#endif
