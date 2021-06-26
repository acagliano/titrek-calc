#ifndef gui_h
#define gui_h

#include "colors.h"
#define gfx_RectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_Rectangle((x1), (y1), (w), (h));
#define gfx_FillRectangleColor(color, x1, y1, w, h) \
    gfx_SetColor((color)); \
    gfx_FillRectangle((x1), (y1), (w), (h));
#define gfx_PrintStringXYC(string, x, y, color) \
    gfx_SetTextFGColor((color)); \
    gfx_PrintStringXY((string), (x), (y));


typedef struct {
    bool length;
    uint8_t* arr;
} exclude_t;

enum SplashOpts {OPT_PLAY, OPT_SETTINGS, OPT_ABOUT, OPT_QUIT};

extern gfx_rletsprite_t* modicons[TARG_SENS + 1];

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t height, bar_colors_t* colors, bool dynamic_bar_color);
void gfx_InitModuleIcons(void);
void gfx_GetVersion(void);
void gfx_VersionCheck(void);
uint8_t gfx_RenderSplash(gfx_rletsprite_t* splash, uint8_t playgame_err);
void gfx_DrawShieldDisp(bool active, uint8_t health, gfx_rletsprite_t* img, uint24_t x, uint8_t y);
void gfx_RenderWindow(window_data_t* win);
bool gui_Login(uint8_t *key);
bool gui_Register(void);

void gfx_RenderMenu(char menutext[][50], uint8_t menucount, uint8_t selected, uint24_t x, uint8_t y, uint24_t w, uint8_t h);
void gfx_RenderMenuOpt(bool active, const char* string, uint24_t x, uint8_t y, uint24_t w, uint8_t h);
#define gfx_RenderVersion(x, y) gfx_RenderMenuTitle((versionstr), (x), (y))

uint8_t prompt_for(char* prompt, char* buffer, size_t len, uint24_t x, uint8_t y, uint8_t flags);

#endif
