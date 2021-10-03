#include <keypadc.h>
#include <fileioc.h>
#include <stdbool.h>
#include <hashlib.h>
#include <compression.h>
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
#include "../gfx/TrekGFX.h"

extern const char *TEMP_PROGRAM;
extern const char *MAIN_PROGRAM;
extern ti_var_t update_fp = 0;
ti_var_t gfx_fp = 0;
extern uint8_t *gfx_appv_name = "TrekGFX";
size_t gfx_dl_size;
size_t gfx_bytes_written = 0;
sha256_ctx gfx_hash;
uint32_t mbuffer[64];

#define TI_PPRGM_T 6

void conn_HandleInput(packet_t *in_buff, size_t buff_size) {
    uint8_t ctl = in_buff->control;
    uint8_t response = in_buff->data[0];    // for handlers needing only response codes
    uint8_t* data = &in_buff->data[0];      // for handlers needing arbitrary data
    if(settings.debug){
        char msg[LOG_LINE_SIZE] = {0};
        sprintf(&msg, "[RECV]: Ctl: %u Len: %u", in_buff[0], buff_size);
        gui_SetLog(LOG_DEBUG, msg);
    }
    
    switch(ctl){
        case WELCOME:
            gui_Login(data);
            break;
        case CONNECT:
            netflags.bridge_up = true;
            gui_SetLog(LOG_INFO, "bridge connect successful");
            ntwk_send(VERSION_CHECK,
                PS_ARR(version),
                PS_ARR(gfx_version)
                );
            break;
        case DISCONNECT:
            gameflags.exit = true;
            break;
        case BRIDGE_ERROR:
            netflags.bridge_error = true;
            break;
        case VERSION_CHECK:
            if(response == VERSION_OK) netflags.client_version_ok = true;
            else gameflags.version_err = true;
            break;
        case LOGIN:
            if(response == SUCCESS) {
                netflags.logged_in = true;
                srv_request_gfx(&gfx_hash, mbuffer);        // see lcars/gui.c
                break;
            }
            else gui_NetworkErrorResponse(ctl, response, true);
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
                    module_t newdata;
                } *packet = (void*)data;
                module_t* thismodule = &Ship.system[packet->slot];
                memcpy(thismodule, &packet->newdata, sizeof(module_t));
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
        case GFX_FRAME_START:               // 91
            hashlib_Sha256Init(&gfx_hash, mbuffer);
            memcpy(&gfx_dl_size, data, sizeof(size_t));
            ntwk_send_nodata(GFX_FRAME_NEXT);
            gfx_bytes_written = 0;
            break;
        case GFX_FRAME_IN:                   // 92
        {
            char msg[LOG_LINE_SIZE] = {0};
            if(!gfx_fp) {if(!(gfx_fp = ti_Open(gfx_appv_name, "w"))) break;}
            if(ti_Write(data, buff_size-1, 1, gfx_fp))
                gfx_bytes_written += buff_size-1;
            hashlib_Sha256Update(&gfx_hash, data, buff_size-1);
            sprintf(msg, "Gfx download :%u%%", (100*gfx_bytes_written/gfx_dl_size));
            gui_SetLog(LOG_INFO, msg);
            ntwk_send_nodata(GFX_FRAME_NEXT);       // 93
            break;
        }
        case GFX_FRAME_DONE:        // 94
            if(gfx_fp){
                uint8_t digest[SHA256_DIGEST_SIZE];
                hashlib_Sha256Final(&gfx_hash, digest);
                if(hashlib_CompareDigest(digest, data, SHA256_DIGEST_SIZE)){
                    ti_SetArchiveStatus(true, gfx_fp);
                    gui_SetLog(LOG_INFO, "download done");
                    gameflags.gfx_error = false;
                    ti_Close(gfx_fp);
                }
                else {
                    gui_SetLog(LOG_ERROR, "download failed");
                    gameflags.gfx_error = true;
                    ti_Close(gfx_fp);
                    ti_Delete(gfx_appv_name);
                    srv_request_gfx(&gfx_hash, mbuffer);        // see lcars/gui.c
                    break;
                }
            }
        case GFX_SKIP:
            gameflags.gfx_loaded = TrekGFX_init();
            if(gameflags.gfx_loaded){
                gui_SetLog(LOG_INFO, "gfx init success");
                gfx_InitModuleIcons();
                gfx_VersionCheck();
                ntwk_send_nodata(LOAD_SHIP);
            }
            break;
        default:
            gui_SetLog(LOG_ERROR, "unknown packet received");
    }
}


