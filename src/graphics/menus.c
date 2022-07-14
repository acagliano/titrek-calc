#include <tice.h>
#include <graphx.h>

#include "../asm/exposure.h"
#include "graphics/animations.h"
#include "graphics/windows.h"
#include "graphics/text.h"
#include "graphics/uiflags.h"
#include "core/version.h"
#include "core/gameloop.h"

char menu_options[4][12] = {
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
}

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
                box_PrintString(&box, menu_options[i], 5, 8, 0);
                box.y+=25;
            }
            splash_render = false;
            gfx_BlitBuffer();
        }
    }
}
