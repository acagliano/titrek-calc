
#ifndef gameloop_h
#define gameloop_h

typedef enum {
    NO_ERROR,
    NO_KEY_FOUND
} game_error_t;
extern game_error_t game_error;

void PlayGame(void);


#endif
