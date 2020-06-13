#ifndef gui_h
#define gui_h

#define wait_kb_release(key) while(kb_IsDown(key)) {ntwk_process(); kb_Scan();}

enum SplashOpts {OPT_PLAY, OPT_SETTINGS, OPT_ABOUT, OPT_QUIT};

extern gfx_rletsprite_t* modicons[TARG_SENS + 1];

void stats_DrawHealthBar(uint24_t health, uint24_t length, uint24_t x, uint8_t y, uint8_t border, uint8_t bg, uint8_t bar);
void gfx_InitModuleIcons(void);
uint8_t gfx_RenderSplash(gfx_rletsprite_t *splash);
void gfx_RenderWindow(uint24_t x, uint8_t y, uint24_t width, uint8_t height, uint8_t borderColor, uint8_t windowColor, uint8_t borderSize);
bool gui_Login(void);
bool gui_Register(void);

#endif
