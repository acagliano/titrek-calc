
#ifndef gameloop_h
#define gameloop_h

typedef enum {
    NO_ERROR,
    KEY_NOT_FOUND,
    KEY_IO_ERROR,
    GFX_LOAD_ERROR,
    NTWK_FAIL_ERROR
} game_error_t;
extern game_error_t game_error;

typedef enum {
    NO_CONNECTION,
    DEVICE_UP,
    BRIDGE_UP,
    SERVER_UP,
    USER_LOGGED_IN
} tick_loop_mode_t;
extern tick_loop_mode_t tick_loop_mode;

typedef enum {
    SCREEN_NONE,
    SCREEN_MAINSYS,
    SCREEN_TACTICALSYS,
    SCREEN_EXTINFO = 0xf0
} screen_selected_t;

typedef struct _lcars_config {
    screen_selected_t screen;
    void* callback_data;
} lcars_config_t;
extern lcars_config_t lcars_config;

extern bool render_frame;
extern bool render_console;

void PlayGame(void);


#endif
