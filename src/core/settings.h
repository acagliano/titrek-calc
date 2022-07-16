
#ifndef settings_h
#define settings_h

typedef struct _config_t {
    uint8_t current;
    uint8_t min;
    uint8_t max;
} config_t;

typedef struct _settings_t {
    bool debug_mode;
    config_t frame_refresh_rate;
    config_t packet_limit;
    uint24_t ntwk_timeout_limit;
} settings_t;
extern settings_t settings;

bool settings_load(void);
bool settings_write(void);

#endif
