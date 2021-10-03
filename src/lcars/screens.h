#ifndef statscreens_h
#define statscreens_h

#include <graphx.h>
#include <stdbool.h>
#include "../classes/ships.h"
#define lcars_texty 223
#define viewer_x 30
#define viewer_y 20
#define xStart 10
#define yStart 14
#define vWidth 300
#define vHeight 170
#define wait_kb_release(key) while(kb_IsDown(key)) {ntwk_process(); kb_Scan();}
#define Screen_ZeroViewport() gfx_SetColor(0); gfx_FillRectangle(23, 17, 273, 140);
#define Screen_ZeroAll() gfx_SetColor(0); gfx_FillRectangle(0, 0, 320, 240);
extern char moduledb[][12];
extern char modulenames[][20];
extern bool full_redraw;

enum ScreenEquates {
    SCRN_OFF = 0,
    SCRN_SENS,
    SCRN_TACT,
    SCRN_MAINS,
    SCRN_TRANSPORT,
    SCRN_CARGO,
    SCRN_NAVIG,
    SCRN_INFO = 0xff00
};

typedef struct {
    uint24_t mains;
    uint24_t tactical;
    uint24_t engine, newspeed;
} selected_t;

typedef struct {
    uint24_t x, w;
    uint8_t y, h, border_color, border_width, bg_color;
} window_data_t;

extern char mainsys_strings[][10];
extern uint16_t screen;
extern selected_t select;

int num_GetLength(int number);
void Screen_RenderUI(void);
//void Screen_UISystemStats(module_t* systems, uint24_t syscount, uint24_t selected);
//void Screen_UITacticalStats(hull_t hull, module_t* systems, uint24_t syscount, uint24_t selected);
//void LCARS_RenderModuleList(module_t* systems, uint24_t syscount, uint8_t class, window_data_t* w, uint24_t selected);
void Screen_UIViewer(void);
void Screen_UIInventory(void);
void Screen_UITransporter(void);
void Screen_UIReplicator(void);
void Screen_UISensors(void);
void Screen_UISpeedConfig(engine_ref_t* eng_ref);
void Overlay_UIModuleInfo(void);
//void Overlay_UIModuleInfo(void);

int Stats_DrawHealthBar(unsigned int percent, unsigned int length, int x, int y, unsigned char border_color, unsigned char bar_color, gfx_rletsprite_t* icon);
void Screen_RenderLCARSTabs(unsigned char active);
void LCARS_RenderModuleList(module_t* systems, uint24_t syscount, uint8_t class, window_data_t* w, uint24_t selected);
void module_RenderGeneral(module_t* module, uint24_t x, uint8_t y, uint24_t width);


#endif
