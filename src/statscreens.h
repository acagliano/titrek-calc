#ifndef statscreens_h
#define statscreens_h

#define lcars_texty 223

void Screen_UISysStats(module_t* system, unsigned char count);
void Screen_UITactStats(void);
void Screen_UIPowerConfig(void);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);

void Screen_Background(unsigned char active);


#endif
