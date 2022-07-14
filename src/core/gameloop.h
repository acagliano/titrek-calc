
#ifndef gameloop_h
#define gameloop_h

typedef enum {
    NO_ERROR,
    KEY_NOT_FOUND,
    KEY_IO_ERROR,
} game_error_t;
extern game_error_t game_error;

typedef enum {
    NO_CONNECTION,
    BRIDGE_CONNECTED,
    SERVER_CONNECTED,
    USER_LOGGED_IN
} tick_loop_mode_t;
extern tick_loop_mode_t tick_loop_mode;

void PlayGame(void);


#endif
