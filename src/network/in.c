#include <keypadc.h>
#include <fileioc.h>
#include <stdbool.h>
#include "../equates.h"
#include "../flags.h"
#include "../versioning.h"
#include "../classes/settings.h"
#include "../classes/player.h"
#include "../classes/ships.h"
#include "controlcodes.h"
#include "network.h"
#include "../lcars/errors.h"
#include "../lcars/gui.h"
#include "../lcars/engine.h"
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
        case CONNECT:
            netflags.bridge_up = true;
            gui_SetLog(LOG_INFO, "bridge connect successful");
            ntwk_send(VERSION_CHECK,
                PS_ARR(version),
                PS_ARR(gfx_version)
                );
            break;
        case DISCONNECT:
            netflags.bridge_up = false;
            netflags.logged_in = false;
            gameflags.loopgame = false;
            break;
        case BRIDGE_ERROR:
            netflags.bridge_error = true;
            break;
        case VERSION_CHECK:
            if(response == VERSION_OK) netflags.client_version_ok = true;
            else gui_SetLog(LOG_SERVER, "client outdated");
            break;
        case REGISTER:
        case LOGIN:
            if(response == SUCCESS) {
                netflags.logged_in = true;
                ntwk_send_nodata(LOAD_SHIP);
                break;
            }
            if((response == MISSING) && (ctl == LOGIN)) gui_Register();
            else {
                memset(&settings.userinfo, 0, sizeof(userinfo_t));
                gui_NetworkErrorResponse(ctl, response, true);
            }
            break;
        case FRAMEDATA_REQUEST:
            {
                uint8_t packet_len = data[0]*8 + 1;
                int x;
                uint8_t y, r;
                for (int i=1; i<packet_len; i+=8){
                    x = 160 + (signed)data[i];
                    y = 120 + (signed)data[i+1];
                    r = data[i+2];
                    gfx_SetColor(data[i+3]);
                    gfx_FillCircle(x, y, r);
                    gfx_SetColor(data[i+4]);
                    gfx_Circle(x, y, r);
                }
            }
            break;
        case MESSAGE:
            gui_SetLog(LOG_SERVER, data);
            break;
        case PRGMUPDATE:
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
        case LOAD_SHIP:
            {
                uint24_t i;
                bool main_set = false, tact_set = false;
                memcpy(&Ship, data, sizeof(ship_t));
                for(i = 0; i < MAX_MODULES; i++){
                    module_t *m = &Ship.system[i];
                    if((m->techclass == mSystem) && !main_set) { select.mains = i; main_set = true;}
                    if((m->techclass == mTactical) && !tact_set) { select.tactical = i; tact_set = true;}
                    if(main_set && tact_set) break;
                }
            }
            break;
        case MODULE_STATE_CHANGE:
            {
                struct {
                    uint8_t slot;
                    uint8_t action;
                    uint8_t value;
                } *packet = (void*)data;
                module_t* thismodule = &Ship.system[packet->slot];
                uint8_t* target = (packet->action == CHANGE_HEALTH) ? &thismodule->health
                    : (packet->action == CHANGE_STATUS_FLAGS) ? &thismodule->status_flags
                    : NULL;
                if(!target) break;
                *target = packet->value;
            }
            break;
        case MODULE_INFO_REQUEST:
            {
                struct {
                    uint8_t slot;
                    moduleinfo_t info;
                } *packet = (void*)data;
                memcpy(&ModuleInfo, &packet->info, sizeof(ModuleInfo));
            }
            break;
        case ENGINE_SETSPEED:
        {
            struct {
                uint8_t slot;
                uint24_t speed;
            } *packet = (void*)data;
            engine_ref.engine[packet->slot].current_speed = packet->speed;
        }
            break;
        case PING:
            ntwk_inactive_clock = 0;
            break;
        case GET_ENGINE_MAXIMUMS:
            memcpy(&engine_ref.engine[0], data, sizeof(engine_ref_t)-1);
            engine_ref.loaded = true;
            break;
        default:
            gui_SetLog(LOG_ERROR, "unknown packet received");
    }
}


