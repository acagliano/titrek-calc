#ifndef flags_h
#define flags_h

typedef struct {
    bool exit;
    bool loopgame;
    bool gfx_error;
    bool gfx_custom;
} flags_t;
extern flags_t gameflags;


#endif
