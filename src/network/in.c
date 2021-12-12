#include <tice.h>
#include <keypadc.h>
#include <fileioc.h>
#include <stdbool.h>
#include <usbdrvce.h>
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
#include "../lcars/text.h"

#define TI_PPRGM_T 6
#define RSA_PUBKEY_LEN 128
#define AES_KEYLEN      32
#define SHA256_DIGEST_SIZE SHA256_DIGEST_LEN

extern const char *TEMP_PROGRAM;
extern const char *MAIN_PROGRAM;
ti_var_t gfx_fp = 0, client_fp = 0;
extern uint8_t *gfx_appv_name = "TrekGFX";
size_t gfx_dl_size, client_dl_size;
size_t gfx_bytes_written = 0, client_bytes_written = 0;
sha256_ctx gfx_hash, client_hash;
uint32_t mbuffer[64];
uint8_t aes_key[AES_KEYLEN] = {0};


#define CEMU_CONSOLE ((char*)0xFB0000)
void hexdump(uint8_t *addr, size_t len, uint8_t *label){
    if(label) sprintf(CEMU_CONSOLE, "\n%s\n", label);
    else sprintf(CEMU_CONSOLE, "\n");
    for(size_t rem_len = len, ct=1; rem_len>0; rem_len--, addr++, ct++){
        sprintf(CEMU_CONSOLE, "%02X ", *addr);
        if(!(ct%AES_BLOCKSIZE)) sprintf(CEMU_CONSOLE, "\n");
    }
    sprintf(CEMU_CONSOLE, "\n");
}


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
        case REQ_SECURE_SESSION:
            {
                struct {
                    size_t kl;
                    uint8_t k[1];
                } *p = (void*)data;
                size_t keylen = p->kl;
                uint8_t* key = &p->k;
                uint8_t msg[256] = {0};
                uint8_t encr_text[32] = {0};
                gfx_TextClearBG("generating AES key...", 20, 190);
                hashlib_RandomBytes(aes_key, AES_KEYLEN);
                sprintf(encr_text, "RSA-%u encrypting...", keylen<<3);
                gfx_TextClearBG(encr_text, 20, 190);
                hexdump(key, keylen, "---RSA Key---");
                hashlib_RSAEncrypt(aes_key, AES_KEYLEN, msg, key, keylen);
                hexdump(msg, keylen, "---Encrypted---");
                ntwk_send(RSA_SEND_SESSION_KEY, PS_PTR(msg, keylen));
                break;
            }
        case RSA_SEND_SESSION_KEY:
            gui_Login(aes_key);
            break;
        case CONNECT:
            netflags.bridge_up = true;
            srv_request_client(&client_hash, mbuffer);
            break;
        case DISCONNECT:
            netflags.logged_in = false;
            gameflags.exit = true;
            break;
        case BRIDGE_ERROR:
            netflags.bridge_error = true;
            break;
        case LOGIN:
            if(response == SUCCESS) {
                netflags.logged_in = true;
                srv_request_gfx(&gfx_hash, mbuffer);        // see lcars/gui.c
            }
            else if(response==INVALID){
                gfx_ErrorClearBG("auth token invalid", 20, 190);
                while(!kb_AnyKey()) kb_Scan();
                gameflags.login_err = true;
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
            if(!gfx_fp) {if(!(gfx_fp = ti_Open("_TrekGFX", "w"))) break;}
            if(ti_Write(data, buff_size-1, 1, gfx_fp))
                gfx_bytes_written += buff_size-1;
            hashlib_Sha256Update(&gfx_hash, data, buff_size-1);
            sprintf(msg, "Gfx download: %u%%", (100*gfx_bytes_written/gfx_dl_size));
            gfx_TextClearBG(msg, 20, 190);
            ntwk_send_nodata(GFX_FRAME_NEXT);       // 93
            break;
        }
        case GFX_FRAME_DONE:        // 94
            if(gfx_fp){
                uint8_t digest[SHA256_DIGEST_SIZE];
                hashlib_Sha256Final(&gfx_hash, digest);
                if(hashlib_CompareDigest(digest, data, SHA256_DIGEST_SIZE)){
                    ti_Delete("TrekGFX");
                    ti_Rename("_TrekGFX", "TrekGFX");
                    ti_SetArchiveStatus(true, gfx_fp);
                    gameflags.gfx_error = false;
                    ti_Close(gfx_fp);
                }
                else {
                    gfx_ErrorClearBG("gfx download error", 20, 190);
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
                gfx_InitModuleIcons();
                gfx_VersionCheck();
                ntwk_send_nodata(LOAD_SHIP);
            }
            break;
        case MAIN_FRAME_START:               // 91
            hashlib_Sha256Init(&client_hash, mbuffer);
            memcpy(&client_dl_size, data, sizeof(size_t));
            ntwk_send_nodata(MAIN_FRAME_NEXT);
            client_bytes_written = 0;
            break;
        case MAIN_FRAME_IN:                   // 92
        {
            char msg[LOG_LINE_SIZE] = {0};
            if(!client_fp) {if(!(client_fp = ti_OpenVar("_TITREK", "w", TI_PPRGM_TYPE))) break;}
            if(ti_Write(data, buff_size-1, 1, client_fp))
                client_bytes_written += buff_size-1;
            hashlib_Sha256Update(&client_hash, data, buff_size-1);
            sprintf(msg, "Client download: %u%%", (100*client_bytes_written/client_dl_size));
            gfx_TextClearBG(msg, 20, 190);
            ntwk_send_nodata(MAIN_FRAME_NEXT);       // 93
            break;
        }
        case MAIN_FRAME_DONE:        // 94
            if(client_fp){
                uint8_t digest[SHA256_DIGEST_SIZE];
                hashlib_Sha256Final(&client_hash, digest);
                if(hashlib_CompareDigest(digest, data, SHA256_DIGEST_SIZE)){
                    ntwk_send_nodata(DISCONNECT);
                    ti_Close(client_fp);
                    gfx_End();
                    usb_Cleanup();
                    ti_DeleteVar("TITREK", TI_PPRGM_TYPE);
                    ti_RenameVar("_TITREK", "TITREK", TI_PPRGM_TYPE);
                    os_RunPrgm("TITREK", NULL, 0, NULL);
                }
                else {
                    gfx_ErrorClearBG("client download error", 20, 190);
                    ti_Close(client_fp);
                    srv_request_client(&client_hash, mbuffer);        // see lcars/gui.c
                    break;
                }
            }
        case MAIN_SKIP:
            gfx_TextClearBG("initiating secure session...", 20, 190);
            ntwk_send_nodata(REQ_SECURE_SESSION);
            break;
        default:
            gui_SetLog(LOG_ERROR, "unknown packet received");
    }
}


