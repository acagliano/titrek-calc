#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <graphx.h>
#include <fontlibc.h>

#include "windows.h"
#include "console.h"

#define CONSOLE_RESERVED_SIZE 1024
#define CONSOLE_BLOCK_SIZE 64
#define CONSOLE_NUM_BLOCKS (CONSOLE_RESERVED_SIZE / CONSOLE_BLOCK_SIZE)

#define CONSOLE_ICONS_NUM 3
#define CONSOLE_ICONS_SIZE 100

char console[CONSOLE_NUM_BLOCKS][CONSOLE_BLOCK_SIZE] = {0};
bool render_console = true;



void console_render(void){
    window_t window = {20, 280, 120, 120};
    window_DrawWindow(&window, 0, 0, 0);
        
    fontlib_SetWindow(window.x, window.y, window.w, window.h);
    fontlib_SetAlternateStopCode(' ');
    fontlib_SetLineSpacing(1, 1);
    fontlib_SetForegroundColor(255);
    fontlib_HomeUp();
    fontlib_SetNewlineOptions(FONTLIB_ENABLE_AUTO_WRAP | FONTLIB_PRECLEAR_NEWLINE | FONTLIB_AUTO_SCROLL);
        
    for(int i = CONSOLE_NUM_BLOCKS-1; i >= 0; i--){
        fontlib_DrawString(console[i]);
    }
    render_console = false;
}

void console_write(console_log_codes code, char* line){
    char log_prefix_str[4];
    char idx=0;
    for(int i=0; i<(CONSOLE_NUM_BLOCKS-1); i++)
        strncpy(console[i], console[i+1], CONSOLE_BLOCK_SIZE);
    
    switch(code){
        case ENTRY_SERVER_MSG:
            strncpy(console[0], "\x01 ", 2);
            break;
        case ENTRY_ERROR_MSG:
            strncpy(console[0], "\x02 ", 2);
            break;
        case ENTRY_DEBUG_MSG:
            strncpy(console[0], "\x03 ", 2);
    }
    if(code > ENTRY_NORMAL) idx = 2;
    
    strncpy(&console[0][idx], line, CONSOLE_BLOCK_SIZE);
    render_console = true;
}
