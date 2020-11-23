#ifndef colors_h
#define colors_h

/*
    This file contains a series of equates for the colors of major elements of the GUI.
    Module backgrounds, healthbar colors, battery indicators
    Edit these to play around with the GUI appearance.
*/

typedef struct {
    uint8_t bar, border, bg;
} bar_colors_t;

// Generally used where I need to drop a black square under a sprite for some reason
#define TRANSPARENT_COLOR 0

// General module colors. Background for online/offline, and border
#define BG_MODULE_ONLINE 3
#define BG_MODULE_OFFLINE 96
#define MODULE_BORDER 229

// Healthbar colors. Mainbar, background (shows under mainbar for <100% health), and border
#define SYS_HEALTHBAR_COLOR 77
#define TACT_HEALTHBAR_COLOR 29
#define ALL_HEALTHBAR_WARN 229
#define ALL_HEALTHBAR_CRITICAL 192
#define ALL_HEALTHBAR_BORDER 0
#define ALL_HEALTHBAR_BG 149

// Healthbar colors for hull integrity. They are a bit different than the normal healthbars.
#define HULLINTEGBAR_COLOR 74
#define HULLINTEGBAR_BORDER 33
#define HULLINTEGBAR_BG 107

// Healthbar colors for combined shield integrity.
#define SHIELDINTEGBAR_COLOR 30
#define SHIELDINTEGBAR_BORDER 18
#define SHIELDINTEGBAR_BG

#endif
