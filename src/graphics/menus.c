#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <stdbool.h>
#include <string.h>
#include <compression.h>
#include <fontlibc.h>

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
                box_PrintStringXY(&box, menu_strings[i], 5, 8, 0);
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
#define skf_SpriteSize 100
bool menu_SelectKeyFile(char* file){
    static bool skf_spritesloaded;
    static uint8_t skf_sprites[skf_NumSprites][skf_SpriteSize];
    gfx_rletsprite_t *server_icon = (gfx_rletsprite_t*)skf_sprites[0],
                    *security_icon = (gfx_rletsprite_t*)skf_sprites[1];
    char *var_name;
    void *vat_ptr = NULL;
    char *filenames = NULL;
    size_t filect = 0, offset=0;
    bool render = true;
    sk_key_t key = 0;
    
    if(!skf_spritesloaded){
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
    
    do {
        key = getKey();
        
        if(key == sk_Clear) { free(filenames); return false; }
        if((key == sk_Left) && (offset>0)) {offset--; render=true;}
        if((key == sk_Right) && (offset<(filect-1))) {offset++; render=true;}
        
        if(render){
            window_t window = {60, 200, 75, 90};
            box_t box = {0};
            static char* skf_enter_flavor = "[Enter] Connect to host";
            static char* skf_left_flavor = "[Left] Prior server";
            static char* skf_right_flavor = "[Right] Next server";
            
            ti_var_t tfp = ti_Open(9*offset+filenames, "r");
            gfx_ZeroScreen();
            
            // main GUI elements
            window_DrawWindow(&window, 149, 24, 3);
            window_DrawTitle(&window, 24, 255, "SERVERS CONFIGURED", false);
            gfx_PrintStringXY(skf_enter_flavor, text_GetCenterX(skf_enter_flavor, 0, 320), 175);
            if(offset>0) gfx_ColoredText(skf_left_flavor, text_GetCenterX(skf_left_flavor, 0, 320), 190, 255);
            else gfx_ColoredText(skf_left_flavor, text_GetCenterX(skf_left_flavor, 0, 320), 190, 74);
            if(offset<(filect-1)) gfx_ColoredText(skf_right_flavor, text_GetCenterX(skf_right_flavor, 0, 320), 205, 255);
            else gfx_ColoredText(skf_right_flavor, text_GetCenterX(skf_right_flavor, 0, 320), 205, 74);
            gfx_SetTextFGColor(0);
            if(tfp) {
                // if appv opened
                size_t filesize = ti_GetSize(tfp);
                size_t keylen = filesize - strlen(ti_GetDataPtr(tfp)+7);
                keylen -= 8; keylen *= 8;
                box_PrintStringXY(&box, "Keyfile: ", 5, 5, 0);
                    gfx_PrintString(9*offset+filenames);
                box_RenderSprite(&box, server_icon, 5, 20);
                box_RenderSprite(&box, security_icon, 5, 35);
                
                box_PrintStringXY(&box, ti_GetDataPtr(tfp)+7, 25, 20, 0);
                box_SetTextXY(&box, 25, 35);
                    gfx_PrintUInt(keylen, num_GetLength(keylen));
                gfx_PrintString("-bit session key");
                box_PrintStringXY(&box, "RSA cipher, key exch", 25, 45, 0);
                box_PrintStringXY(&box, "AES cipher, login", 25, 55, 0);
                
                ti_SetArchiveStatus(true, tfp);
                ti_Close(tfp);
            }
            else {
                gfx_PrintStringXY("Error opening file", 10, 25);
            }
            gfx_BlitBuffer();
            render = false;
        }
    } while (key != sk_Enter);
    strcpy(file, 9*offset+filenames);
    free(filenames);
    return true;
}
