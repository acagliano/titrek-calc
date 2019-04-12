// convpng v7.0
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekgui__
#define __trekgui__
#include <stdint.h>

#include <stdbool.h>

#define trekgui_num 8

extern uint8_t *trekgui[8];
#define shipinterior_width 320
#define shipinterior_height 240
#define shipinterior ((gfx_sprite_t*)trekgui[0])
#define icon_health_width 11
#define icon_health_height 11
#define icon_health ((gfx_sprite_t*)trekgui[1])
#define icon_online_width 11
#define icon_online_height 11
#define icon_online ((gfx_sprite_t*)trekgui[2])
#define icon_offline_width 11
#define icon_offline_height 11
#define icon_offline ((gfx_sprite_t*)trekgui[3])
#define icon_sourcemain_width 11
#define icon_sourcemain_height 11
#define icon_sourcemain ((gfx_sprite_t*)trekgui[4])
#define icon_sourcereserve_width 11
#define icon_sourcereserve_height 11
#define icon_sourcereserve ((gfx_sprite_t*)trekgui[5])
#define icon_powerdraw_width 11
#define icon_powerdraw_height 11
#define icon_powerdraw ((gfx_sprite_t*)trekgui[6])
#define icon_powerspend_width 11
#define icon_powerspend_height 11
#define icon_powerspend ((gfx_sprite_t*)trekgui[7])
bool trekgui_init(void *decompressed_addr);
#define trekgui_uncompressed_size 77663
#define trekgui_compressed_size 3316

#endif
