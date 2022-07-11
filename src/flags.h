#ifndef flags_h
#define flags_h

typedef struct {
    bool exit;
    bool loopgame;
    bool gfx_loaded;
    bool gfx_error;
    bool gfx_custom;
    bool login_err;
    bool version_err;
} flags_t;
extern flags_t gameflags;


typedef struct {
    uint8_t server;
} bridge_config_t;
extern bridge_config_t bridge_config;

#endif
