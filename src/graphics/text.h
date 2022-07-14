
#ifndef text_h
#define text_h

int text_GetCenterX(char* string, uint24_t start_x, uint24_t width);
void text_PaddedText(char* string, uint24_t x, uint8_t y, uint8_t bgcolor, uint8_t textcolor, uint8_t padding, uint8_t scale);

#endif
