// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekgui__
#define __trekgui__
#include <stdint.h>

#include <stdbool.h>

#define trekgui_num 25

extern uint8_t *trekgui[25];
#define tactical_width 20
#define tactical_height 41
#define tactical_compressed ((gfx_sprite_t*)trekgui[0])
#define power_alert_width 20
#define power_alert_height 20
#define power_alert_compressed ((gfx_sprite_t*)trekgui[1])
#define ship_alert_width 60
#define ship_alert_height 29
#define ship_alert_compressed ((gfx_sprite_t*)trekgui[2])
#define hull_alert_width 60
#define hull_alert_height 29
#define hull_alert_compressed ((gfx_sprite_t*)trekgui[3])
#define hull_normal_width 60
#define hull_normal_height 29
#define hull_normal_compressed ((gfx_sprite_t*)trekgui[4])
#define torpedo_alert_width 20
#define torpedo_alert_height 20
#define torpedo_alert_compressed ((gfx_sprite_t*)trekgui[5])
#define lifesupport_alert_width 20
#define lifesupport_alert_height 20
#define lifesupport_alert_compressed ((gfx_sprite_t*)trekgui[6])
#define breach_alert_width 20
#define breach_alert_height 20
#define breach_alert_compressed ((gfx_sprite_t*)trekgui[7])
#define splashlogo_width 65
#define splashlogo_height 100
#define splashlogo_compressed ((gfx_sprite_t*)trekgui[8])
#define shield_dmg_icon_width 10
#define shield_dmg_icon_height 10
#define shield_dmg_icon_compressed ((gfx_sprite_t*)trekgui[9])
#define hull_dmg_icon_width 10
#define hull_dmg_icon_height 10
#define hull_dmg_icon_compressed ((gfx_sprite_t*)trekgui[10])
#define splash_dmg_icon_width 10
#define splash_dmg_icon_height 10
#define splash_dmg_icon_compressed ((gfx_sprite_t*)trekgui[11])
#define health_icon_width 10
#define health_icon_height 10
#define health_icon_compressed ((gfx_sprite_t*)trekgui[12])
#define power_icon_width 10
#define power_icon_height 10
#define power_icon_compressed ((gfx_sprite_t*)trekgui[13])
#define frequency_icon_width 10
#define frequency_icon_height 10
#define frequency_icon_compressed ((gfx_sprite_t*)trekgui[14])
#define drv_icon_width 10
#define drv_icon_height 10
#define drv_icon_compressed ((gfx_sprite_t*)trekgui[15])
#define impulsespeed_icon_width 21
#define impulsespeed_icon_height 16
#define impulsespeed_icon_compressed ((gfx_sprite_t*)trekgui[16])
#define warpspeed_icon_width 23
#define warpspeed_icon_height 16
#define warpspeed_icon_compressed ((gfx_sprite_t*)trekgui[17])
#define battery_icon_width 10
#define battery_icon_height 10
#define battery_icon_compressed ((gfx_sprite_t*)trekgui[18])
#define ship_icon_width 16
#define ship_icon_height 16
#define ship_icon_compressed ((gfx_sprite_t*)trekgui[19])
#define range_icon_width 12
#define range_icon_height 12
#define range_icon_compressed ((gfx_sprite_t*)trekgui[20])
#define target_width 30
#define target_height 30
#define target_compressed ((gfx_sprite_t*)trekgui[21])
#define senslock_width 12
#define senslock_height 12
#define senslock_compressed ((gfx_sprite_t*)trekgui[22])
#define ship_select_1_width 94
#define ship_select_1_height 52
#define ship_select_1_compressed ((gfx_sprite_t*)trekgui[23])
#define ship_select_2_width 98
#define ship_select_2_height 48
#define ship_select_2_compressed ((gfx_sprite_t*)trekgui[24])
bool trekgui_init(void);

#endif
