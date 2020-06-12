
#ifndef errorscreens_h
#define errorscreens_h

#include <stddef.h>

void gui_NetworkErrorResponse(uint8_t controlcode, uint8_t responsecode);
void gui_DebugMsg(char type, char data);

#endif
