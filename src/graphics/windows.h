#ifndef windows_h
#define windows_h


typedef struct _window_t {
    uint24_t x, w;
    uint8_t y, h;
} window_t;

typedef struct _box_t {
    uint24_t x, w;
    uint8_t y, h;
} box_t;


#define MAINMENU_BGCOLOR 149
#define MAINMENU_BORDERCOLOR 24
#define MAINMENU_BORDERWIDTH 3
#define MENU_SELECTED_BGCOLOR 116

void window_DrawWindow(window_t* window, uint8_t bgcolor, uint8_t bordercolor, uint8_t borderwidth);
void window_DrawTitle(window_t* window, uint8_t titlebgcolor, uint8_t titlefontcolor, const char* title, bool largetext);
void box_ParentToWindow(box_t* box, window_t* window);
void box_DrawBox(box_t* box, uint8_t bgcolor, uint8_t bordercolor, uint8_t borderwidth);
void box_PrintStringXY(box_t* box, const char* string, uint24_t x, uint8_t y, uint8_t color);
void box_RenderSprite(box_t* box, gfx_rletsprite_t* sprite, uint24_t x, uint8_t y);
void box_SetTextXY(box_t* box, uint24_t x, uint8_t y);

#endif
