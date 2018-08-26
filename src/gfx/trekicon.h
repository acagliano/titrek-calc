// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __trekicon__
#define __trekicon__
#include <stdint.h>

#include <stdbool.h>

#define trekicon_num 9

extern uint8_t *trekicon[9];
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
bool trekicon_init(void);

#endif
