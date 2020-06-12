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
#include "equates.h"
#include "classes/ships.h"
#include "classes/coords.h"
#include "classes/tech.h"
#include "classes/screens.h"
#include "statscreens.h"
#include "gfx/TrekGFX.h"
#include "gfx-engine/gui.h"
#include "gfx/internal.h"
#include "errorscreens.h"
#include "asm/exposure.h"

// USB Libraries
#include <usbdrvce.h>
#include <srldrvce.h>

#define DEBUG
#undef NDEBUG
#include <debug.h>

#include "network/usb.h"

#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

char version[] = {0, 0, 91, ALPHA};
char *versionstr = "0.0.91 alpha";
ship_t Ship = {0};
selected_t select = {0, 0};
gfx_rletsprite_t* gfx_sprites;
bool debug = 0;
gfx_UninitedRLETSprite(splash, splash_size);
gfx_UninitedRLETSprite(err_icon, icon_internalerr_size);
flags_t gameflags = {0};

srl_device_t srl;
uint8_t srl_buf[2048];

/* Main Menu */

void PlayGame(void);

void MainMenu(void) {
    uint8_t opt = 0;
    while(1){
        opt = gfx_RenderSplash(splash);
        if(opt == OPT_PLAY) {gameflags.loopgame = 1; PlayGame();}
        if(opt == OPT_QUIT) {gameflags.exit = 1; break;}
        if(opt == OPT_ABOUT) {
            gfx_ZeroScreen();
            gfx_SetColor(239);
            gfx_PrintStringXY("## About Project TI-Trek ##", 5, 5);
            gfx_PrintStringXY("A multiplayer space combat game", 10, 20);
            gfx_PrintStringXY("for the TI-84+ CE!", 10, 30);
            gfx_PrintStringXY("_Authors_", 10, 40);
            gfx_PrintStringXY("ACagliano - lead, client", 15, 50);
            gfx_PrintStringXY("beck - lead, server", 15, 60);
            gfx_PrintStringXY("command - lead, USB bridge", 15, 70);
            gfx_PrintStringXY("(c) 2019, Project TI-Trek", 5, 230);
            gfx_BlitBuffer();
            while(!kb_IsDown(kb_KeyClear)) kb_Scan();
        }
        if(opt == OPT_SETTINGS){}
    }
}



/* Handle USB events */
static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
								  usb_callback_data_t *callback_data) {
	/* When a device is connected, or when connected to a computer */
	if(event == USB_DEVICE_CONNECTED_EVENT || event == USB_HOST_CONFIGURE_EVENT) {
	    usb_device_t device = event_data;
	    char dummy;
        srl_error_t srl_error = srl_Init(&srl, device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY);
        if(!srl_error) srl_SetRate(&srl, 115200);
        if(!srl_error) srl_Read(&srl, &dummy, 1);
        if(!srl_error) {
            gameflags.network = true;
        }
	}

	/* When a device is disconnected */
	if(event == USB_DEVICE_DISCONNECTED_EVENT) {
        gameflags.network = false;
	}

	return USB_SUCCESS;
}

int main(void) {
    usb_error_t usb_error;
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    zx7_Decompress(splash, splash_compressed);
    zx7_Decompress(err_icon, icon_internalerr_compressed);

    gameflags.network = false;
    usb_error = usb_Init(handle_usb_event, NULL, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS);
    if(usb_error) goto error;

    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    do {
        MainMenu();
    } while(!gameflags.exit);
error:
    usb_Cleanup();
    gfx_End();
    pgrm_CleanUp();
    return 0;
}


void PlayGame(void){
    /* A buffer for internal use by the serial library */
    gfx_rletsprite_t *compr_src, *decomp_dest;
    ti_var_t assets;
    uint16_t screen = 0;
    size_t current_size = 0;
    char in_buff[1024];
    if(!gameflags.network) return;
    if(!ntwk_Login()) {
        dbg_sprintf(dbgout, "Failed to login\n");
        return;
    }
    if(!TrekGFX_init()) return;
    gfx_InitModuleIcons();
    do {
        /* A buffer to store bytes read by the serial library */
        size_t bytes_read;
        sk_key_t key = getKey();
        Screen_RenderUI(screen, &Ship, &select);
        if(!gameflags.logged_in) gui_NetworkErrorResponse(3, 6);
        gfx_BlitBuffer();
        if(key == sk_Clear){
            if(screen > 0xff) screen = resbits(screen, SCRN_INFO);
            else if(gameflags.logged_in) ntwk_Disconnect();
            else gameflags.loopgame = false;
        }
        if(key == sk_Stat) debug == true;
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

        usb_HandleEvents();

        /* If the device was disconnected, exit */
        if(!gameflags.network) return;
        /* Handle input */
        if(current_size) {
          if(srl_Available(&srl) >= current_size) {
            if(srl_Read(&srl, in_buff, current_size) == current_size){
                gui_NetworkErrorResponse(3, 7);
                gfx_BlitBuffer();
                while(!kb_AnyKey()) kb_Scan();
            }
            conn_HandleInput((usb_packet_t *) &in_buff, current_size);
            current_size = 0;
          }
        } else {
          if(srl_Available(&srl) >= 3) srl_Read(&srl, (void*)&current_size, 3);
        }

    } while(gameflags.loopgame);
}
