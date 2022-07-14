
#include <graphx.h>

int text_GetCenterX(char* string, uint24_t start_x, uint24_t width){
    return start_x + ((width - gfx_GetStringWidth(string))>>1);
}

void text_PaddedText(char* string, uint24_t x, uint8_t y, uint8_t bgcolor, uint8_t textcolor, uint8_t padding, uint8_t scale){
    uint24_t strwidth = gfx_GetStringWidth(string) * scale;
    uint8_t oldtext = gfx_SetTextFGColor(textcolor);
    gfx_SetTextScale(scale, scale);
    gfx_SetColor(bgcolor);
    gfx_FillRectangle(x - padding, y - padding, strwidth + padding + padding, 8 * scale + padding + padding);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextScale(1, 1);
    gfx_SetTextFGColor(oldtext);
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100) + (number >= 1000) + (number >= 10000);
}

void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color){
    uint8_t oldcolor = gfx_SetTextFGColor(color);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextFGColor(oldcolor);
}
