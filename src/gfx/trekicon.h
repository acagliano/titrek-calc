// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekicon__
#define __trekicon__
#include <stdint.h>

#include <stdbool.h>

#define trekicon_num 20

extern uint8_t *trekicon[20];
#define tactical_width 20
#define tactical_height 41
#define tactical_compressed ((gfx_sprite_t*)trekicon[0])
#define power_alert_width 20
#define power_alert_height 20
#define power_alert_compressed ((gfx_sprite_t*)trekicon[1])
#define ship_alert_width 60
#define ship_alert_height 29
#define ship_alert_compressed ((gfx_sprite_t*)trekicon[2])
#define hull_alert_width 60
#define hull_alert_height 29
#define hull_alert_compressed ((gfx_sprite_t*)trekicon[3])
#define hull_normal_width 60
#define hull_normal_height 29
#define hull_normal_compressed ((gfx_sprite_t*)trekicon[4])
#define torpedo_alert_width 20
#define torpedo_alert_height 20
#define torpedo_alert_compressed ((gfx_sprite_t*)trekicon[5])
#define lifesupport_alert_width 20
#define lifesupport_alert_height 20
#define lifesupport_alert_compressed ((gfx_sprite_t*)trekicon[6])
#define breach_alert_width 20
#define breach_alert_height 20
#define breach_alert_compressed ((gfx_sprite_t*)trekicon[7])
#define splashlogo_width 65
#define splashlogo_height 100
#define splashlogo_compressed ((gfx_sprite_t*)trekicon[8])
#define shield_dmg_icon_width 10
#define shield_dmg_icon_height 10
#define shield_dmg_icon_compressed ((gfx_sprite_t*)trekicon[9])
#define hull_dmg_icon_width 10
#define hull_dmg_icon_height 10
#define hull_dmg_icon_compressed ((gfx_sprite_t*)trekicon[10])
#define splash_dmg_icon_width 10
#define splash_dmg_icon_height 10
#define splash_dmg_icon_compressed ((gfx_sprite_t*)trekicon[11])
#define health_icon_width 10
#define health_icon_height 10
#define health_icon_compressed ((gfx_sprite_t*)trekicon[12])
#define power_icon_width 10
#define power_icon_height 10
#define power_icon_compressed ((gfx_sprite_t*)trekicon[13])
#define frequency_icon_width 10
#define frequency_icon_height 10
#define frequency_icon_compressed ((gfx_sprite_t*)trekicon[14])
#define drv_icon_width 10
#define drv_icon_height 10
#define drv_icon_compressed ((gfx_sprite_t*)trekicon[15])
#define impulsespeed_icon_width 21
#define impulsespeed_icon_height 16
#define impulsespeed_icon_compressed ((gfx_sprite_t*)trekicon[16])
#define warpspeed_icon_width 23
#define warpspeed_icon_height 16
#define warpspeed_icon_compressed ((gfx_sprite_t*)trekicon[17])
#define battery_icon_width 10
#define battery_icon_height 10
#define battery_icon_compressed ((gfx_sprite_t*)trekicon[18])
#define ship_icon_width 16
#define ship_icon_height 16
#define ship_icon_compressed ((gfx_sprite_t*)trekicon[19])
bool trekicon_init(void);

#endif
