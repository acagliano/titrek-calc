#ifndef frames_h
#define frames_h

#include "../gamestate.h"

void frame_render_headerbar(const char *title);
void frame_screen_up(uint8_t screen_up);
void screendata_init(void);


enum _msg_types {
	MSG_NORMAL = 255,
	MSG_WARNING = 229,
	MSG_ERROR = 224
};
void console_insert_line(const char *line, uint8_t msg_type);

#endif
