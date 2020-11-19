
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <graphx.h>
#define LINE_SPACING 10

// text_WrappedString(STRING, WRAPAT, X, Y)
// Returns, number of lines rendered
uint8_t text_WrappedString(char *str, uint24_t left_margin, uint8_t top_margin, uint24_t right_margin){
    char *word = str;
    char *current = str;
    uint8_t lines = 1;

    gfx_SetTextXY(left_margin, top_margin);

    while(true) {
        uint8_t width = 0;
        for(; *current != ' ' && *current != 0; current++) {
            width += gfx_GetCharWidth(*current);
        }

        if(gfx_GetTextX() + width > right_margin) {
            gfx_SetTextXY(left_margin, gfx_GetTextY() + LINE_SPACING);
            lines++;
        }

        for(; word <= current; word++) {
            gfx_PrintChar(*word);
        }

        if(*current == 0) break;
        current++;
    }
    return lines;
}

uint8_t text_GetStringHeight(char *str, uint24_t line_length){
    uint8_t lines = 1;
    char *current = str;
    uint24_t x = 0;
    while(true) {
        uint24_t width = (lines == 1) ? 12 : 0;
        for(; *current != ' ' && *current != 0; current++) {
            width += gfx_GetCharWidth(*current);
        }
        x += width;
        if(x > line_length) {
            lines++;
            x = width;
        }

        if(*current == 0) break;
        current++;
    }
    return lines;
}


