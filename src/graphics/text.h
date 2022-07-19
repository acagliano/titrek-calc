
#ifndef text_h
#define text_h

int text_GetCenterX(char* string, uint24_t start_x, uint24_t width);
void text_PaddedText(char* string, uint24_t x, uint8_t y, uint8_t bgcolor, uint8_t textcolor, uint8_t padding, uint8_t scale);
int num_GetLength(int number);
void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color);
uint8_t text_WrappedString(char *str, uint24_t left_margin, uint8_t top_margin, uint24_t right_margin);
uint8_t text_GetStringHeight(char *str, uint24_t line_length);
char* fontlib_RenderAll(const char* string, uint8_t fontcolor, bool xcolor);

#endif
