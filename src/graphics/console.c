#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <graphx.h>
#include <fontlibc.h>
#include <stdio.h>

#include "windows.h"
#include "console.h"
#include "text.h"

#define CONSOLE_RESERVED_SIZE 1024
#define CONSOLE_BLOCK_SIZE 64
#define CONSOLE_NUM_BLOCKS (CONSOLE_RESERVED_SIZE / CONSOLE_BLOCK_SIZE)

#define CONSOLE_ICONS_NUM 3
#define CONSOLE_ICONS_SIZE 100

char console[CONSOLE_RESERVED_SIZE] = {0};
bool render_console = true;
char *console_readto = NULL;


void console_render(void){

    if(console_readto == NULL) return;

    char* render_this = console;
    window_t window = {20, 280, 120, 120};
    window_DrawWindow(&window, 0, 0, 0);
        
    fontlib_SetWindow(window.x, window.y, window.w, window.h);
    fontlib_SetAlternateStopCode(' ');
    fontlib_SetLineSpacing(1, 1);
    fontlib_SetForegroundColor(255);
    fontlib_HomeUp();
    fontlib_SetNewlineOptions(FONTLIB_ENABLE_AUTO_WRAP | FONTLIB_PRECLEAR_NEWLINE | FONTLIB_AUTO_SCROLL);
        
    while(render_this < console_readto)
        render_this = fontlib_RenderAll(render_this, 255, true);
    
    render_console = false;
    fontlib_SetWindowFullScreen();
}

void console_write(uint8_t code, char* line){
    static bool console_init = false;
    static char *console_start, *console_end, *console_writein;
    char *console_clear = console_start;
    size_t stringlen = (sizeof code) + (sizeof code) + strlen(line);
    size_t deletelen = 0;
    
    if(line == NULL) return;
    if(!strlen(line))  return;
    if(code > 3) return;
    
    if(!console_init){
        console_start = console;
        console_end = &console[CONSOLE_RESERVED_SIZE-1];
        console_writein = console_start;
        console_init = true;
    }
    
    while((console_writein + stringlen - deletelen) > console_end){
        deletelen += strlen(console_clear) + 1;
        console_clear = console_start + deletelen;
    }
    memcpy(console_start, console_clear, console_writein - console_start);
    console_writein -= deletelen;
    
    if(code) console_writein += sprintf(console_writein, "%c ", code);
        
    console_writein += sprintf(console_writein, "%s%c\n", line, 4);
    console_writein++;
    
    console_readto = console_writein;
    
    render_console = true;
}
