#ifndef settings_h
#define settings_h

#include <stdint.h>
#include <stdbool.h>
#include "player.h"

#define num_settings 7
extern char *settingsappv;

typedef struct {
    uint8_t chunk_refresh;  // how many ticks to skip before asking for more frame data
    uint8_t entity_refresh; // .. .. entity data
    uint8_t packet_limit;   // max # of packets to recv() per tick
    uint24_t network_timeout;
    uint24_t log_limit;
} limiter_t;


#define LOGIN_TOKEN_SIZE 128
typedef struct {
    bool debug;
    limiter_t limits;
    char servers[10][50];
    uint8_t login_key[LOGIN_TOKEN_SIZE];
} settings_t;
extern settings_t settings;

void set_defaults(void);
bool write_settings(void);

enum SettingOpt {
    DEBUG_MODE,
    CHUNK_REF,
    PACKET_LIMIT,
    NTWK_TIMEOUT,
    LOG_TIME
};
#define NUM_SETTINGS (LOG_TIME+1)
bool check_import_login_key(void);

#endif
