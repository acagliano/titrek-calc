// convpng v7.0
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekgui__
#define __trekgui__
#include <stdint.h>

#include <stdbool.h>

#define trekgui_num 15

extern uint8_t *trekgui[15];
#define shipinterior_left_width 160
#define shipinterior_left_height 240
#define shipinterior_left ((gfx_rletsprite_t*)trekgui[0])
#define shipinterior_right_width 160
#define shipinterior_right_height 240
#define shipinterior_right ((gfx_rletsprite_t*)trekgui[1])
#define icon_shieldsstable_width 96
#define icon_shieldsstable_height 48
#define icon_shieldsstable ((gfx_rletsprite_t*)trekgui[2])
#define icon_shieldscollapse_width 96
#define icon_shieldscollapse_height 48
#define icon_shieldscollapse ((gfx_rletsprite_t*)trekgui[3])
#define icon_shieldsfail_width 96
#define icon_shieldsfail_height 48
#define icon_shieldsfail ((gfx_rletsprite_t*)trekgui[4])
#define icon_shieldsoff_width 64
#define icon_shieldsoff_height 32
#define icon_shieldsoff ((gfx_rletsprite_t*)trekgui[5])
#define icon_health_width 11
#define icon_health_height 11
#define icon_health ((gfx_rletsprite_t*)trekgui[6])
#define icon_online_width 11
#define icon_online_height 11
#define icon_online ((gfx_rletsprite_t*)trekgui[7])
#define icon_offline_width 11
#define icon_offline_height 11
#define icon_offline ((gfx_rletsprite_t*)trekgui[8])
#define icon_sourcemain_width 11
#define icon_sourcemain_height 11
#define icon_sourcemain ((gfx_rletsprite_t*)trekgui[9])
#define icon_sourcereserve_width 11
#define icon_sourcereserve_height 11
#define icon_sourcereserve ((gfx_rletsprite_t*)trekgui[10])
#define icon_powerdraw_width 11
#define icon_powerdraw_height 11
#define icon_powerdraw ((gfx_rletsprite_t*)trekgui[11])
#define icon_powerspend_width 11
#define icon_powerspend_height 11
#define icon_powerspend ((gfx_rletsprite_t*)trekgui[12])
#define icon_frequency_width 11
#define icon_frequency_height 11
#define icon_frequency ((gfx_rletsprite_t*)trekgui[13])
#define icon_shieldstrength_width 11
#define icon_shieldstrength_height 11
#define icon_shieldstrength ((gfx_rletsprite_t*)trekgui[14])
bool trekgui_init(void *decompressed_addr);
#define trekgui_uncompressed_size 36859
#define trekgui_compressed_size 4554

#endif
