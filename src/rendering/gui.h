#ifndef gui_h
#define gui_h

#include "colors.h"

#define wait_kb_release(key) while(kb_IsDown(key)) {ntwk_process(); kb_Scan();}

enum SplashOpts {OPT_PLAY, OPT_SETTINGS, OPT_ABOUT, OPT_QUIT};

extern gfx_rletsprite_t* modicons[TARG_SENS + 1];

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t height, bar_colors_t* colors);
void gfx_InitModuleIcons(void);
void gfx_GetVersion(void);
void gfx_VersionCheck(void);
uint8_t gfx_RenderSplash(gfx_rletsprite_t* splash);
void gfx_DrawShieldDisp(bool active, uint8_t health, gfx_rletsprite_t* img, uint24_t x, uint8_t y);
void gfx_RenderWindow(uint24_t x, uint8_t y, uint24_t width, uint8_t height, uint8_t borderColor, uint8_t windowColor, uint8_t borderSize);
bool gui_Login(void);
bool gui_Register(void);

void gfx_RenderMenu(char menutext[][50], uint8_t menucount, uint8_t selected, uint24_t x, uint8_t y, uint24_t w, uint8_t h);
void gfx_RenderMenuOpt(bool active, const char* string, uint24_t x, uint8_t y, uint24_t w, uint8_t h);
#define gfx_RenderVersion(x, y) gfx_RenderMenuTitle((versionstr), (x), (y))

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y, uint8_t flags);

#endif
