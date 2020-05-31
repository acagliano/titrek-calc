//--------------------------------------
// Program Name: Star Trek Multplayer
// Author: Anthony Cagliano
// License:
// Description:
//--------------------------------------

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <intce.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h, debug.h
#include <fileioc.h>
#include <graphx.h>
#include <compression.h>
#include <keypadc.h>

// Game Data Types (Structure Definitions)

#include "classes/ships.h"
#include "classes/coords.h"
#include "classes/tech.h"
#include "classes/screens.h"
#include "statscreens.h"
#include "gfx/trekgui.h"
#include "gfx-engine/gui.h"
#include "gfx/internal.h"
#define usb_callback_data_t usb_device_t
#include <usbdrvce.h>
#include <srldrvce.h>
/* Put your function prototypes here */
#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

ship_t Ship = {0};
selected_t select = {0, 0};

/* Get the usb_device_t for each newly attached device */
static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                  usb_callback_data_t *callback_data) {
    if(event == USB_DEVICE_CONNECTED_EVENT || event == USB_HOST_CONFIGURE_EVENT) {
        dbg_sprintf(dbgout, "Device connected.\n");
        if(!*callback_data) {
            *callback_data = event_data;
        }
    }
    if(event == USB_DEVICE_DISCONNECTED_EVENT) {
        dbg_sprintf(dbgout, "Device disconnected.\n");
        *callback_data = NULL;
    }
    return USB_SUCCESS;
}


void main(void) {
    usb_error_t error = 0;
    usb_device_t dev = NULL;
    srl_device_t srl;
    static char srlbuf[4096];
    usb_Init(handle_usb_event, &dev, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS));
    while(!dev) {
        uint8_t val;
        kb_Scan();
        if(kb_IsDown(kb_KeyClear)) {
            goto exit;
        }
        val = usb_HandleEvents();
        if(val)
            dbg_sprintf(dbgerr, "error in HandleEvents %u\n", val);
    }
    uint16_t screen = 0;
    bool loopgame = true;
    unsigned int i;
    ti_var_t appvar;
    uint8_t opt = 0;
    gfx_rletsprite_t* gfx_sprites = malloc(trekgui_uncompressed_size);
    gfx_rletsprite_t* splash = malloc(splash_width * splash_height);
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    if(!gfx_sprites) return;
    if(!(appvar = ti_Open("trekgui", "r"))) return;
    zx7_Decompress(gfx_sprites, ti_GetDataPtr(appvar));
    zx7_Decompress(splash, splash_compressed);
    trekgui_init(gfx_sprites);
    gfx_InitModuleIcons();
    ti_CloseAll();
    int_Disable();
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    while(1){
        opt = gfx_RenderSplash(splash);
        if(opt == OPT_PLAY) break;
        if(opt == OPT_QUIT) {loopgame = false; break;}
        if(opt == OPT_ABOUT || opt == OPT_SETTINGS){}
    }
    module_SetHealthMax(&Ship.hull.health, 1000);
    for(i = 0; i < SYS_MAX; i++){
       // Ship.system[i].techclass = MC_SYSTEM;
        Ship.system[i].online = true;
        Ship.system[i].modclass = mSystem;
        Ship.system[i].techtype = i;
        module_SetHealthMax(&Ship.system[i].health, 50);
        module_SetPowerMax(&Ship.system[i].power);
    }
    for(i = 0; i < 2; i++){
      //  Ship.shield[i].techclass = MC_TACTICAL;
        Ship.system[SYS_MAX+i].unlocked = true;
        Ship.system[SYS_MAX+i].modclass = mTactical;
        Ship.system[SYS_MAX+i].online = true;
        Ship.system[SYS_MAX+i].techtype = SHIELD;
        module_SetHealthMax(&Ship.system[SYS_MAX+i].health, 50);
        module_SetPowerMax(&Ship.system[SYS_MAX+i].power);
        Ship.system[SYS_MAX+i].data.mod_shields.resistance = 10;
    }
    for(i = 0; i < MAX_MODULES; i++)
        // set system module first selection
        if(Ship.system[i].modclass == mSystem) {
            select.mains = i;
            break;
        }
    for(i = 0; i < MAX_MODULES; i++)
           // set tactical module first selection
           if(Ship.system[i].modclass == mTactical) {
               select.tactical = i;
               break;
           }
    do {
        unsigned char key = os_GetCSC();
        Screen_RenderUI(screen, &Ship, &select);
        if(key == sk_Clear){
            if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
            else loopgame = false;
        }
        if(key == sk_Yequ)
            screen = (screen == SCRN_SENS) ? SCRN_OFF : SCRN_SENS;
        if(key == sk_Window)
            screen = (screen == SCRN_TACT) ? SCRN_OFF : SCRN_TACT;
        if(key == sk_Zoom)
            screen = (screen == SCRN_MAINS) ? SCRN_OFF : SCRN_MAINS;
        if(key == sk_Trace)
            screen = (screen == SCRN_TRANSPORT) ? SCRN_OFF : SCRN_TRANSPORT;
        if(key == sk_Graph)
            screen = (screen == SCRN_CARGO) ? SCRN_OFF : SCRN_CARGO;
        if(key == sk_Enter){
            if((screen == SCRN_MAINS) || (screen == SCRN_TACT))
                screen = setbits(screen, SCRN_INFO);
        }
        if(key == sk_Square){
            char i;
            char modnum = randInt(0, SYS_MAX - 1);
            char randnum = randInt(5, 10);
            for(i = 0; i < MAX_MODULES; i++){
                module_t *shield = &Ship.system[i];
                if((shield->techtype == SHIELD) && shield->online){
                    int health = health_GetHealthPercent(&shield->health);
                    int drv = shield->data.mod_shields.resistance;
                    drv = health * drv / 100;
                    health_DamageModule(&shield->health, -(randnum));
                    randnum -= drv;
                }
            }
            health_DamageModule(&Ship.hull.health, -(randnum));
            health_DamageModule(&Ship.system[modnum].health, -(randnum));
        }
        if(key == sk_Down) {
            char i;
            switch(screen){
                case SCRN_TACT:
                    for(i = select.tactical + 1; i < (TACT_MAX + SYS_MAX - 1); i++){
                        int type = Ship.system[i].modclass;
                        if( type == mTactical ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case SCRN_MAINS:
                    for(i = select.mains + 1; i < (TACT_MAX + SYS_MAX - 1); i++){
                        int type = Ship.system[i].modclass;
                        if( type == mSystem ){
                            select.mains = i;
                            break;
                        }
                    }
                    break;
            }
        }
        if(key == sk_Up){
            char i;
            switch(screen){
                case SCRN_TACT:
                    for(i = select.tactical - 1; i >= 0; i--){
                        int type = Ship.system[i].modclass;
                        if( type == mTactical ){
                            select.tactical = i;
                            break;
                        }
                    }
                    break;
                case SCRN_MAINS:
                    for(i = select.mains - 1; i >= 0; i--){
                        int type = Ship.system[i].modclass;
                        if( type == mSystem ){
                            select.mains = i;
                            break;
                        }
                    }
                    break;
            }
        }
            
    } while(loopgame == true);
exit:
    if(error) {
        char buf[4];
        sprintf(buf, "%u: 0x%X\n", step, error);
        fontlib_DrawString(buf);
        dbg_sprintf(dbgerr, "error %u\n", error);
        while(!kb_IsDown(kb_KeyClear)) kb_Scan();
    }
    usb_Cleanup();
    free(gfx_sprites);
    gfx_End();
    int_Enable();
    pgrm_CleanUp();
    return;
}

/* Put other functions here */
/* Draw text on the homescreen at the given X/Y location */
