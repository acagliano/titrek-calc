#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20

void Screen_UISysStats(module_t* system, unsigned char count);
void Screen_UITactStats(void);
void Screen_UIPowerConfig(void);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);

void Screen_Background(gfx_sprite_t **sprites, unsigned char active);


#endif
