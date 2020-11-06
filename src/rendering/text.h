
#ifndef text_h
#define text_h

uint8_t text_WrappedString(char *str, uint24_t left_margin, uint8_t top_margin, uint24_t right_margin);
uint8_t text_GetStringHeight(char *str, uint24_t line_length);
char *strify_version(char *str, uint8_t *version);

#endif
