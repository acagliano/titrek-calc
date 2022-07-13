
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <graphx.h>
#include <tice.h>
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

int text_GetCenterX(char* string){
    return (LCD_WIDTH - gfx_GetStringWidth(string)) / 2;
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100) + (number >= 1000) + (number >= 10000);
}

uint8_t gfx_VCenterText(uint8_t y, uint8_t box_height, uint8_t font_height){
    uint8_t padding = box_height-font_height;
    return padding/2+y;
}

void gfx_HighlightedText(const char* string, uint24_t x, uint8_t y, uint8_t text_color, uint8_t highlight_color){
    uint24_t width = gfx_GetStringWidth(string);
    uint8_t oldcolor = gfx_SetTextFGColor(text_color);
    gfx_SetColor(highlight_color);
    gfx_FillRectangle(x, y, width + 16, 12);
    gfx_PrintStringXY(string, x + 8, y + 2);
    gfx_SetTextFGColor(oldcolor);
}

void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color){
    uint8_t oldcolor = gfx_SetTextFGColor(color);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextFGColor(oldcolor);
}

void gfx_TextClearBG(const char* string, uint24_t x, uint8_t y, bool full_line){
    uint8_t old_gfx_color = gfx_SetColor(0);
    if(full_line) gfx_FillRectangle(0, y, 320, 8);
    else gfx_FillRectangle(x, y, gfx_GetStringWidth(string), 8);
    gfx_PrintStringXY(string, x, y);
    gfx_SetColor(old_gfx_color);
    gfx_BlitRectangle(gfx_buffer, x, y, 320, 8);
}

void gfx_ErrorClearBG(const char* error, uint24_t x, uint8_t y){
    uint8_t old_gfx_color = gfx_SetColor(0);
    uint24_t width = gfx_GetStringWidth(error);
    gfx_FillRectangle(x, y, 320, 8);
    gfx_SetColor(224);
    gfx_FillRectangle(x-5, y-1, width+10, 10);
    gfx_PrintStringXY(error, x, y);
    gfx_SetColor(old_gfx_color);
    gfx_BlitRectangle(gfx_buffer, x-5, y-1, 320, 10);
}

void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color){
    uint8_t oldcolor = gfx_SetTextFGColor(color);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextFGColor(oldcolor);
}
