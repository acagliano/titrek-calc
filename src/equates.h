#ifndef equates_h
#define equates_h

#include <srldrvce.h>
#include <stdbool.h>

#define xStart 10
#define yStart 14
#define vWidth 300
#define vHeight 170

extern srl_device_t srl;

typedef struct {
    bool network;
    bool logged_in;
} flags_t;

#endif
