
#ifndef text_h
#define text_h

uint8_t text_WrappedString(char *str, uint24_t left_margin, uint8_t top_margin, uint24_t right_margin);
uint8_t text_GetStringHeight(char *str, uint24_t line_length);
int text_GetCenterX(char* string);
int num_GetLength(int number);
uint8_t gfx_VCenterText(uint8_t y, uint8_t box_height, uint8_t font_height);
void gfx_HighlightedText(const char* string, uint24_t x, uint8_t y, uint8_t text_color, uint8_t highlight_color);
void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color);
void gfx_TextClearBG(const char* string, uint24_t x, uint8_t y, bool full_line);
void gfx_ErrorClearBG(const char* error, uint24_t x, uint8_t y);
#endif
