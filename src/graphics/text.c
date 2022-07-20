#include <string.h>

#include <graphx.h>
#include <fontlibc.h>
#define LINE_SPACING 10

int text_GetCenterX(char* string, uint24_t start_x, uint24_t width){
    return start_x + ((width - gfx_GetStringWidth(string))>>1);
}

void text_PaddedText(char* string, uint24_t x, uint8_t y, uint8_t bgcolor, uint8_t textcolor, uint8_t padding){
    size_t strwidth = fontlib_GetStringWidth(string);
    fontlib_SetForegroundColor(textcolor);
    gfx_SetColor(bgcolor);
    gfx_FillRectangle(x - padding, y - padding, strwidth + padding + padding, 8 + padding + padding);
    fontlib_SetCursorPosition(x, y);
    fontlib_DrawString(string);
}

int num_GetLength(int number){
    return 1 + (number >= 10) + (number >= 100) + (number >= 1000) + (number >= 10000);
}

void gfx_ColoredText(const char* string, uint24_t x, uint8_t y, uint8_t color){
    uint8_t oldcolor = gfx_SetTextFGColor(color);
    gfx_PrintStringXY(string, x, y);
    gfx_SetTextFGColor(oldcolor);
}

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

char* fontlib_RenderAll(const char* string, uint8_t fontcolor, bool xcolor){
    char* render = string;
    uint24_t x_last;
    uint8_t printmin = fontlib_GetFirstPrintableCodePoint();
    fontlib_SetAlternateStopCode(' ');
    fontlib_SetNewlineOptions(FONTLIB_ENABLE_AUTO_WRAP | FONTLIB_PRECLEAR_NEWLINE | FONTLIB_AUTO_SCROLL);
    do {
        x_last = fontlib_DrawString(render);
        render = fontlib_GetLastCharacterRead();
        if(*render < printmin){
            if(xcolor){
                uint8_t n_color;
                switch(*render){
                    case 1:
                        n_color = 159;
                        break;
                    case 2:
                        n_color = 224;
                        break;
                    case 3:
                        n_color = 247;
                        break;
                    case 4:
                        n_color = fontcolor;
                }
                fontlib_SetForegroundColor(n_color);
                if(*render < 4) fontlib_DrawGlyph(*render);
            }
        }
        render++;
        if((fontlib_GetStringWidth(render) + x_last) >= 310) fontlib_Newline();
        else fontlib_DrawGlyph(0);
    } while(render < (string + strlen(string)));
    fontlib_SetAlternateStopCode(0);
    fontlib_SetNewlineOptions(0);
    return render;
}
