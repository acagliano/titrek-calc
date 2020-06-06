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

// USB Libraries
#define usb_callback_data_t usb_device_t
#include <usbdrvce.h>
#include <srldrvce.h>

#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

ship_t Ship = {0};
selected_t select = {0, 0};
usb_device_t usb_device = NULL;
srl_device_t srl;
gfx_UninitedRLETSprite(gfx_sprites, trekgui_uncompressed_size);

/* Main Menu */

void PlayGame(void);

void MainMenu(void) {
    uint8_t opt = 0;
    gfx_UninitedRLETSprite(splash, splash_width * splash_height);
    zx7_Decompress(splash, splash_compressed);
    while(1){
        opt = gfx_RenderSplash(splash);
            if(opt == OPT_PLAY) PlayGame();
            if(opt == OPT_QUIT) break;
            if(opt == OPT_ABOUT || opt == OPT_SETTINGS){}
        }
    }



/* Handle USB events */
static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
								  usb_callback_data_t *callback_data) {
	/* When a device is connected, or when connected to a computer */
	if(event == USB_DEVICE_CONNECTED_EVENT || event == USB_HOST_CONFIGURE_EVENT) {
		if(!*callback_data) {
			/* Set the USB device */
			*callback_data = event_data;
		}
	}

	/* When a device is disconnected */
	if(event == USB_DEVICE_DISCONNECTED_EVENT) {
		*callback_data = NULL;
	}

	return USB_SUCCESS;
}

void main(void) {
    usb_error_t usb_error;
    uint8_t usb_timeout = 0xff;
    srl_error_t srl_error;
    uint8_t srl_buf[2048];
    unsigned int i;
    gfx_Begin();
    srandom(rtc_Time());
    ti_CloseAll();
    int_Disable();
    
    usb_error = usb_Init(handle_usb_event, &usb_device, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS);
    if(usb_error) goto error;
    
    while(!usb_device && usb_timeout--) {
           kb_Scan();

           /* Exit if clear is pressed */
           if(kb_IsDown(kb_KeyClear)) {
               goto error;
           }
           /* Handle any USB events that have occured */
           usb_HandleEvents();
       }
    srl_error = srl_Init(&srl, usb_device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY);
    if(srl_error) goto error;
    gfx_SetDefaultPalette(gfx_8bpp);
    gfx_SetDrawBuffer();
    gfx_SetTextTransparentColor(1);
    gfx_SetTextBGColor(1);
    MainMenu();
error:
    usb_Cleanup();
    gfx_End();
    int_Enable();
    pgrm_CleanUp();
    return;
}


void PlayGame(void){
    /* A buffer for internal use by the serial library */
    ti_var_t appvar;
    uint16_t screen = 0;
    bool loopgame = true;
    if(!gfx_sprites) return;
    if(!(appvar = ti_Open("trekgui", "r"))) return;
    zx7_Decompress(gfx_sprites, ti_GetDataPtr(appvar));
    trekgui_init(gfx_sprites);
    gfx_InitModuleIcons();

    /* Wait for a USB device to be connected */
   

    /* Initialize the serial library with the USB device */
    do {
        /* A buffer to store bytes read by the serial library */
        char in_buf[64];
        size_t bytes_read;
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

        usb_HandleEvents();

        /* If the device was disconnected, exit */
        if(!usb_device) break; // here, can we `goto` the main menu?
        /* Read up to 64 bytes from serial */
        bytes_read = srl_Read(&srl, in_buf, sizeof(in_buf));
        /* Only write when bytes were received */
        if(bytes_read) {
            /* Write the same bytes back to the other device */
            srl_Write(&srl, in_buf, bytes_read);
        }

    } while(loopgame);
}
