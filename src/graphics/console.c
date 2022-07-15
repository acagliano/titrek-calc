#include <stdint.h>
#include <stdbool.h>

#define CONSOLE_RESERVED_SIZE 1024
#define CONSOLE_BLOCK_SIZE 64
#define CONSOLE_NUM_BLOCKS (CONSOLE_RESERVED_SIZE / CONSOLE_BLOCK_SIZE)

#define CONSOLE_ICONS_NUM 3
#define CONSOLE_ICONS_SIZE 100

char console[CONSOLE_NUM_BLOCKS][CONSOLE_BLOCK_SIZE] = {0};
bool render_console = true;

enum _entry_speccodes {
    ENTRY_NORMAL = 0xfffff0;
    ENTRY_SERVER_MSG,
    ENTRY_ERROR_MSG,
    ENTRY_DEBUG_MSG
}

void console_Render(void){
    static uint8_t console_icons[CONSOLE_ICONS_NUM][CONSOLE_ICONS_SIZE];
    static bool console_icons_loaded;
    gfx_rletsprite_t *console_icon_server = (gfx_rletsprite_t*)console_icons[0],
                    *console_icon_error = (gfx_rletsprite_t*)console_icons[1],
                    *console_icon_debug = (gfx_rletsprite_t*)console_icons[2];
                    
    if(!console_icons_loaded){
        zx7_Decompress(console_icon_server, log_server_compressed);
        zx7_Decompress(console_icon_error, log_error_compressed);
        zx7_Decompress(console_icon_debug, log_error_compressed);
        console_icons_loaded = true;
    }
    if(render_console){
        for(int i = 0; i<CONSOLE_NUM_BLOCKS; i++){
            uint24_t type = *(&console[i][0]);
            type &= 0x00000f;
            char* line = &console[i][sizeof type];
        }
}

void console_Write(char* line){
    for(int i=0; i<9; i++)
        strncpy(console[i], console[i+1], CONSOLE_BLOCK_SIZE);
    strcpy(console[0], )
}
