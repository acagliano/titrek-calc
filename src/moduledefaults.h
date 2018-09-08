
#ifndef moduledefaults_h
#define moduledefaults_h

#include "datatypes/shipmodules.h"

#define shields ((Module_t*)&ShipModules[tt_shield-1])
#define integrity ((Module_t*)&ShipModules[tt_integrity-1])
#define lifesupport ((Module_t*)&ShipModules[tt_lifesupport-1])
#define warpdrive ((Module_t*)&ShipModules[tt_warpdrive-1])
#define impulsedrive ((Module_t*)&ShipModules[tt_impulsedrive-1])
#define phaser ((Module_t*)&ShipModules[tt_phaser-1])
#define torpedo ((Module_t*)&ShipModules[tt_torpedo-1])
#define sensors ((Module_t*)&ShipModules[tt_sensor-1])
#define transporter ((Module_t*)&ShipModules[tt_transporter-1])
#define auxpower ((Module_t*)&ShipModules[tt_auxiliary-1])
#define warpcore ((Module_t*)&ShipModules[tt_warpcore-1])

#endif
