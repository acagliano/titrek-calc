#ifndef uxassets_appvar_include_file
#define uxassets_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define guiicons_palette_offset 0
#define deflected_width 16
#define deflected_height 16
#define uxassets_guiicons_deflected_index 0
#define deflected ((gfx_rletsprite_t*)uxassets_appvar[0])
#define engspeed_display_width 24
#define engspeed_display_height 24
#define uxassets_guiicons_engspeed_display_index 1
#define engspeed_display ((gfx_rletsprite_t*)uxassets_appvar[1])
#define icon_frequency_width 11
#define icon_frequency_height 11
#define uxassets_guiicons_icon_frequency_index 2
#define icon_frequency ((gfx_rletsprite_t*)uxassets_appvar[2])
#define icon_health_width 11
#define icon_health_height 11
#define uxassets_guiicons_icon_health_index 3
#define icon_health ((gfx_rletsprite_t*)uxassets_appvar[3])
#define icon_hullinteg_width 10
#define icon_hullinteg_height 10
#define uxassets_guiicons_icon_hullinteg_index 4
#define icon_hullinteg ((gfx_rletsprite_t*)uxassets_appvar[4])
#define icon_power_ok_width 10
#define icon_power_ok_height 10
#define uxassets_guiicons_icon_power_ok_index 5
#define icon_power_ok ((gfx_rletsprite_t*)uxassets_appvar[5])
#define icon_power_warn_width 10
#define icon_power_warn_height 10
#define uxassets_guiicons_icon_power_warn_index 6
#define icon_power_warn ((gfx_rletsprite_t*)uxassets_appvar[6])
#define icon_powerdraw_width 11
#define icon_powerdraw_height 11
#define uxassets_guiicons_icon_powerdraw_index 7
#define icon_powerdraw ((gfx_rletsprite_t*)uxassets_appvar[7])
#define icon_powerspend_width 11
#define icon_powerspend_height 11
#define uxassets_guiicons_icon_powerspend_index 8
#define icon_powerspend ((gfx_rletsprite_t*)uxassets_appvar[8])
#define icon_shieldinteg_width 10
#define icon_shieldinteg_height 10
#define uxassets_guiicons_icon_shieldinteg_index 9
#define icon_shieldinteg ((gfx_rletsprite_t*)uxassets_appvar[9])
#define icon_sourcemain_width 11
#define icon_sourcemain_height 11
#define uxassets_guiicons_icon_sourcemain_index 10
#define icon_sourcemain ((gfx_rletsprite_t*)uxassets_appvar[10])
#define icon_sourcereserve_width 11
#define icon_sourcereserve_height 11
#define uxassets_guiicons_icon_sourcereserve_index 11
#define icon_sourcereserve ((gfx_rletsprite_t*)uxassets_appvar[11])
#define mains_display_width 24
#define mains_display_height 24
#define uxassets_guiicons_mains_display_index 12
#define mains_display ((gfx_rletsprite_t*)uxassets_appvar[12])
#define shipinterior_left_width 160
#define shipinterior_left_height 240
#define uxassets_guiicons_shipinterior_left_index 13
#define shipinterior_left ((gfx_rletsprite_t*)uxassets_appvar[13])
#define shipinterior_right_width 160
#define shipinterior_right_height 240
#define uxassets_guiicons_shipinterior_right_index 14
#define shipinterior_right ((gfx_rletsprite_t*)uxassets_appvar[14])
#define tactical_display_width 24
#define tactical_display_height 24
#define uxassets_guiicons_tactical_display_index 15
#define tactical_display ((gfx_rletsprite_t*)uxassets_appvar[15])
#define particles_palette_offset 0
#define cracked_width 32
#define cracked_height 32
#define uxassets_particles_cracked_index 16
#define cracked ((gfx_rletsprite_t*)uxassets_appvar[16])
#define ship_palette_offset 0
#define side_width 24
#define side_height 24
#define uxassets_ship_side_index 17
#define side ((gfx_rletsprite_t*)uxassets_appvar[17])
#define top_width 24
#define top_height 24
#define uxassets_ship_top_index 18
#define top ((gfx_rletsprite_t*)uxassets_appvar[18])
#define uxassets_entries_num 19
extern unsigned char *uxassets_appvar[19];
unsigned char uxassets_init(void);

#ifdef __cplusplus
}
#endif

#endif
