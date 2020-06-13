#ifndef settings_h
#define settings_h

#include <stdint.h>
#include <stdbool.h>
#include "player.h"

extern char *settingsappv;

typedef struct {
    uint8_t chunk_refresh;  // how many ticks to skip before asking for more frame data
    uint8_t entity_refresh; // .. .. entity data
    uint8_t packet_limit;   // max # of packets to recv() per tick
} limiter_t;


typedef struct {
    userinfo_t userinfo;
    limiter_t limits;
} settings_t;
extern settings_t settings;

void set_defaults(void);
bool write_settings(void);

#endif
