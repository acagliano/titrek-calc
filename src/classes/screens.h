#ifndef screens_h
#define screens_h

enum ScreenEquates {
    SCRN_OFF = 0,
    SCRN_SENS,
    SCRN_TACT,
    SCRN_MAINS,
    SCRN_TRANSPORT,
    SCRN_CARGO,
    SCRN_INFO = 0xff00
};

typedef struct {
    uint24_t mains;
    uint24_t tactical;
} selected_t;

#endif
