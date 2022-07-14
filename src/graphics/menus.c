#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <stdbool.h>
#include <string.h>
#include <compression.h>

#include "../asm/exposure.h"
#include "animations.h"
#include "windows.h"
#include "text.h"
#include "uiflags.h"
#include "../core/version.h"
#include "../core/gameloop.h"
#include "../gfx/internal.h"

void menu_Settings(void);
void About(void);

char menu_strings[4][12] = {
    "PLAY GAME",
    "SETTINGS",
    "ABOUT",
    "EXIT"
};

enum _menu_options {
    PLAY,
    SETTINGS,
    ABOUT,
    EXIT
} menu_options;

bool splash_render = true;

void menu_MainMenu(void){
    sk_key_t key = 0;
    int8_t selected = 0;
    window_t window = {40, 130, 80, 100};
    while(1){
        key = getKey();
        anim_MainMenu(0, 0, 320, 240);
        if(key == sk_Down) selected += (selected < 3);
        if(key == sk_Up) selected -= (selected > 0);
        if(key == sk_Clear) break;
        if(key == sk_Enter) {
            if(selected==PLAY) PlayGame();
            else if(selected==SETTINGS) menu_Settings();
            else if(selected==ABOUT) About();
            else if(selected==EXIT) break;
        }
        if(key || splash_render) {
            box_t box = {0, 0, 0, 25};
            window_DrawWindow(&window, MAINMENU_BGCOLOR, MAINMENU_BORDERCOLOR, MAINMENU_BORDERWIDTH);
            window_DrawTitle(&window, MAINMENU_BORDERCOLOR, 255, "TI-TREK", true);
            box_ParentToWindow(&box, &window);
            text_PaddedText(version, 170-55, 187, 24, 255, 5, 1);
            for(int i=0; i<4; i++){
                uint8_t boxcol = (selected == i) ? MENU_SELECTED_BGCOLOR : MAINMENU_BGCOLOR;
                box_DrawBox(&box, boxcol, 0, 1);
                box_PrintString(&box, menu_strings[i], 5, 8, 0);
                box.y+=25;
            }
            splash_render = false;
            gfx_BlitBuffer();
        }
    }
}

void menu_Settings(void){}

void About(void){}

#define CEMU_CONSOLE ((char*)0xFB0000)
#define skf_NumSprites 2
#define skf_SpriteSize (9*9)
bool menu_SelectKeyFile(char* file){
    static bool skf_spritesloaded = false;
    static uint8_t skf_sprites[skf_NumSprites][skf_SpriteSize];
    char *var_name;
    void *vat_ptr = NULL;
    char *filenames = NULL;
    size_t filect = 0, offset=0;
    bool render = true;
    sk_key_t key = 0;
    gfx_rletsprite_t *server_icon = (gfx_rletsprite_t*)skf_sprites[0],
                    *security_icon = (gfx_rletsprite_t*)skf_sprites[1];
    
    if(!skf_spritesloaded){
        sprintf(CEMU_CONSOLE, "loading sprites\n");
        zx7_Decompress(server_icon, log_server_compressed);
        zx7_Decompress(security_icon, icon_security_compressed);
        skf_spritesloaded = true;
    }

    while ((var_name = ti_Detect(&vat_ptr, "TrekKey"))){
        filenames = realloc(filenames, sizeof(filenames)+9);
        strncpy(9*filect+filenames, var_name, 9);
        filect++;
    }
    
    if(filenames==NULL) return false;
    sprintf(CEMU_CONSOLE, "generated names\n");
    do {
        char* hostinfo;
        key = getKey();
        
        if(key == sk_Clear) return false;
        if((key == sk_Left) && (offset>0)) {offset--; render=true;}
        if((key == sk_Right) && (offset<(filect-1))) {offset++; render=true;}
        
        if(render){
            window_t window = {3, 300, 20, 90};
            box_t box = {0};
            
            ti_var_t tfp = ti_Open(9*offset+filenames, "r");
            gfx_ZeroScreen();
            
            box_ParentToWindow(&box, &window);
            // main GUI elements
            sprintf(CEMU_CONSOLE, "drawing gui\n");
            window_DrawWindow(&window, 195, 2, 181);
            window_DrawTitle(&window, 24, 255, "SERVERS CONFIGURED", true);
            gfx_PrintStringXY("[Enter] Connect to selected host", 5, 120);
            if(offset>0) gfx_ColoredText("[Left] Select previous server", 5, 130, 255);
            else gfx_ColoredText("[Left] Select previous server", 5, 130, 74);
            if(offset<(filect-1)) gfx_ColoredText("[Right] Select next server", 5, 140, 255);
            else gfx_ColoredText("[Right] Select next server", 5, 140, 74);
            sprintf(CEMU_CONSOLE, "drawing server info\n");
            gfx_SetTextFGColor(0);
            if(tfp) {
                // if appv opened
                size_t filesize = ti_GetSize(tfp);
                size_t keylen = filesize - strlen(ti_GetDataPtr(tfp)+7);
                keylen -= 8; keylen *= 8;
                gfx_PrintStringXY("Keyfile: ", 10, 35);
                gfx_PrintString(9*offset+filenames);
                gfx_RLETSprite(server_icon, 10, 50);
                gfx_RLETSprite(security_icon, 10, 65);
                gfx_PrintStringXY(ti_GetDataPtr(tfp)+7, 25, 50);
                gfx_SetTextXY(25, 65);
                gfx_PrintUInt(keylen, num_GetLength(keylen));
                gfx_PrintString("-bit session key");
                gfx_PrintStringXY("RSA-<=2048 cipher, key exch", 25, 75);
                gfx_PrintStringXY("AES-256 cipher, login", 25, 85);
                
                ti_Close(tfp);
            }
            else {
                gfx_PrintStringXY("Error opening file", 10, 25);
            }
            gfx_BlitBuffer();
            render = false;
        }
        free(filenames);
    } while (key != sk_Enter);
    strcpy(file, 9*offset+filenames);
    free(filenames);
    return true;
}
