
#ifndef console_h
#define console_h

#include <stdint.h>

typedef enum _entry_speccodes {
    ENTRY_NORMAL = 1,
    ENTRY_SERVER_MSG,
    ENTRY_ERROR_MSG,
    ENTRY_DEBUG_MSG
} console_log_codes;

void console_render(void);
void console_write(uint8_t code, char* line);

#endif
