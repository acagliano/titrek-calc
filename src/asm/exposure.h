#ifndef exposure_h
#define exposure_h

#include <stddef.h>
#include <stdint.h>
#include <keypadc.h>

char user_input(char* buffer, size_t length, uint8_t flags);
uint8_t getKey(void);
void enableInts(void);
void disableInts(void);


#endif
