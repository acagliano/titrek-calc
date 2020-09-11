#include <keypadc.h>
#include <fileioc.h>
#include <stdbool.h>
#include "../equates.h"
#include "../rendering/errors.h"
#include "../classes/settings.h"
#include "../classes/player.h"
#include "../classes/ships.h"
#include "controlcodes.h"
#include "network.h"
#include "../rendering/gui.h"
#include "../rendering/engine.h"
#include "../asm/exposure.h"

extern const char *TEMP_PROGRAM;
extern const char *MAIN_PROGRAM;
extern ti_var_t update_fp;

#define TI_PPRGM_T 6

void conn_HandleInput(packet_t *in_buff, size_t buff_size) {
    uint8_t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    
    switch(ctl){
        case REGISTER:
        case LOGIN:
            if(response == SUCCESS) {
                gameflags.logged_in = true;
                ntwk_send_nodata(LOADSHIP);
                break;
            }
            if((response == MISSING) && (ctl == LOGIN)) gui_Register();
            else {
                memset(&settings.userinfo, 0, sizeof(userinfo_t));
                gui_NetworkErrorResponse(ctl, response, true);
            }
            break;
        case DISCONNECT:
            gameflags.logged_in = false;
            break;
        case REQCHUNK:
            renderFrame((body_packet_t*)data);
            break;
        case MESSAGE:
            // to handle
            break;
        case PGRMUPDATE:
            if (!update_fp){
                update_fp = ti_OpenVar(TEMP_PROGRAM,"w",TI_PPRGM_T);
            }
            if (buff_size<2){
                ti_Close(update_fp);
                ti_DeleteVar(MAIN_PROGRAM,TI_PPRGM_T);
                ti_RenameVar(TEMP_PROGRAM,MAIN_PROGRAM,TI_PPRGM_T);
                update_program();
            }
            ti_Write(data, buff_size-1, 1, update_fp);
            break;
        case LOADSHIP:
            memcpy(&Ship, data, sizeof(ship_t));
            break;
        default:
            gui_NetworkErrorResponse(3, 7, true);
    }
}


