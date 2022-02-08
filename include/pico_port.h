#ifndef PICO_PORT_H
#define PICO_PORT_H

#include <debug.h>
#include <stdlib.h>
#include <tice.h>

#define dbg printf
#define pico_zalloc(size) calloc(size, 1)
#define pico_free free

#define PICO_TIME() (timer_Get(1) / 32768)
#define PICO_TIME_MS() ((uint32_t)(timer_Get(1) / 32.768))

void PICO_IDLE(void);

#endif //PICO_PORT_H
