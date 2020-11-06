
#ifndef errorscreens_h
#define errorscreens_h

#include <stddef.h>
#define LOG_LINE_SIZE 100

void gui_NetworkErrorResponse(uint8_t controlcode, uint8_t responsecode, bool blocking);
void gui_SetLog(uint8_t type, const char* msg);
void gui_ShowLog(void);

enum LogType {
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG,
    LOG_SERVER
};
extern char log_msg[4][LOG_LINE_SIZE];
extern gfx_rletsprite_t* icon[4];
extern uint24_t log_display[4];

#endif
