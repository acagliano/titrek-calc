// convpng v7.0
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekgui__
#define __trekgui__
#include <stdint.h>

#include <stdbool.h>

#define trekgui_num 17

extern uint8_t *trekgui[17];
#define shipinterior_left_width 160
#define shipinterior_left_height 240
#define shipinterior_left ((gfx_rletsprite_t*)trekgui[0])
#define shipinterior_right_width 160
#define shipinterior_right_height 240
#define shipinterior_right ((gfx_rletsprite_t*)trekgui[1])
#define icon_shieldsstable_width 70
#define icon_shieldsstable_height 35
#define icon_shieldsstable ((gfx_rletsprite_t*)trekgui[2])
#define icon_shieldscollapse_width 70
#define icon_shieldscollapse_height 35
#define icon_shieldscollapse ((gfx_rletsprite_t*)trekgui[3])
#define icon_shieldsfail_width 70
#define icon_shieldsfail_height 35
#define icon_shieldsfail ((gfx_rletsprite_t*)trekgui[4])
#define icon_shieldsoff_width 70
#define icon_shieldsoff_height 35
#define icon_shieldsoff ((gfx_rletsprite_t*)trekgui[5])
#define icon_health_width 11
#define icon_health_height 11
#define icon_health ((gfx_rletsprite_t*)trekgui[6])
#define icon_sourcemain_width 11
#define icon_sourcemain_height 11
#define icon_sourcemain ((gfx_rletsprite_t*)trekgui[7])
#define icon_sourcereserve_width 11
#define icon_sourcereserve_height 11
#define icon_sourcereserve ((gfx_rletsprite_t*)trekgui[8])
#define icon_powerdraw_width 11
#define icon_powerdraw_height 11
#define icon_powerdraw ((gfx_rletsprite_t*)trekgui[9])
#define icon_powerspend_width 11
#define icon_powerspend_height 11
#define icon_powerspend ((gfx_rletsprite_t*)trekgui[10])
#define icon_frequency_width 11
#define icon_frequency_height 11
#define icon_frequency ((gfx_rletsprite_t*)trekgui[11])
#define deflected_width 16
#define deflected_height 16
#define deflected ((gfx_rletsprite_t*)trekgui[12])
#define tactical_display_width 24
#define tactical_display_height 24
#define tactical_display ((gfx_rletsprite_t*)trekgui[13])
#define mains_display_width 24
#define mains_display_height 24
#define mains_display ((gfx_rletsprite_t*)trekgui[14])
#define icon_hullinteg_width 10
#define icon_hullinteg_height 10
#define icon_hullinteg ((gfx_rletsprite_t*)trekgui[15])
#define icon_shieldinteg_width 10
#define icon_shieldinteg_height 10
#define icon_shieldinteg ((gfx_rletsprite_t*)trekgui[16])
bool trekgui_init(void *decompressed_addr);
#define trekgui_uncompressed_size 35078
#define trekgui_compressed_size 5668

#endif
