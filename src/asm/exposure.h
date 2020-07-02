#ifndef exposure_h
#define exposure_h

#include <stddef.h>
#include <stdint.h>
#include <keypadc.h>

char user_input(char* buffer, size_t length, uint8_t flags);
uint8_t getKey(void);

bool cemu_check(void);
size_t cemu_get(void *buf, size_t size);
void cemu_send(void *buf, size_t size);

void update_program(void);

#endif
