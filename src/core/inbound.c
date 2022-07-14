
#include <tice.h>
#include <keypadc.h>
#include <fileioc.h>
#include <stdbool.h>
#include <usbdrvce.h>
#include <hashlib.h>
#include <compression.h>
#include "controlcodes.h"
#include "network.h"

#define SHA256_DIGEST_SIZE SHA256_DIGEST_LEN

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
    
    static const char *temp_program = "_TITREK";
    static const char *main_program = "TITREK";
    static const char *temp_gfx = "_TrekGFX";
    static const char *main_gfx = "TrekGFX";
    static ti_var_t filehandle;
    static size_t file_dl_size;
    static size_t file_bytes_written;
    static hash_ctx filestream_hash;
    
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
                gfx_TextClearBG("generating AES key...", 20, 190, true);
                csrand_fill(aes_key, AES_KEYLEN);
                sprintf(encr_text, "RSA-%u encrypting...", keylen<<3);
                gfx_TextClearBG(encr_text, 20, 190, true);
                hexdump(key, keylen, "---RSA Key---");
                rsa_encrypt(aes_key, AES_KEYLEN, msg, key, keylen, SHA256);
                hexdump(msg, keylen, "---Encrypted---");
                ntwk_send(RSA_SEND_SESSION_KEY, PS_PTR(msg, keylen));
                break;
            }
        case RSA_SEND_SESSION_KEY:
            gui_Login(aes_key);
            break;
        case CONNECT:
            netflags.bridge_up = true;
            srv_request_client(&filestream_hash);
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
                srv_request_gfx(&filestream_hash);        // see lcars/gui.c
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
        case MAIN_FRAME_START:
        case GFX_FRAME_START:               // 91
        {
            const char* file = (ctl==MAIN_FRAME_START) ? temp_program : temp_gfx;
            uint8_t out_ctl_code = (ctl==MAIN_FRAME_START) ? MAIN_FRAME_NEXT : GFX_FRAME_NEXT;
            hash_init(&filestream_hash, SHA256);
            memcpy(&file_dl_size, data, sizeof(size_t));
            file_bytes_written = 0;
            if(!(filehandle = ti_Open(file, "w"))) break;
            ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
            ntwk_send();
            break;
        }
        case MAIN_FRAME_IN:
        case GFX_FRAME_IN:                   // 92
        {
            char msg[LOG_LINE_SIZE] = {0};
            char fname[9] = {0};
            uint8_t out_ctl_code = (ctl==MAIN_FRAME_IN) ? MAIN_FRAME_NEXT : GFX_FRAME_NEXT;
            if(!filehandle) break;
            ti_GetName(fname, filehandle);
            if(ti_Write(data, buff_size-1, 1, gfx_fp))
                file_bytes_written += buff_size-1;
            hash_update(&filestream_hash, data, buff_size-1);
            sprintf(msg, "%s download: %u%%", fname, (100*gfx_bytes_written/gfx_dl_size));
            gfx_TextClearBG(msg, 20, 190, true);
            ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
            ntwk_send();
            break;
        }
        case MAIN_FRAME_DONE:
        case GFX_FRAME_DONE:        // 94
        {
            char fname[9] = {0};
            if(!filehandle) break;
            ti_GetName(fname, filehandle)
            uint8_t digest[SHA256_DIGEST_SIZE];
            hash_final(&filestream_hash, digest);
            if(digest_compare(digest, data, SHA256_DIGEST_SIZE)){
                const char* filein = (ctl==MAIN_FRAME_DONE) ? temp_program : temp_gfx;
                const char* fileout = (ctl==MAIN_FRAME_DONE) ? main_program : main_gfx;
                uint8_t filetype = (ctl==MAIN_FRAME_DONE) ? OS_TYPE_TMP_PRGM : OS_TYPE_APPVAR;
                ti_DeleteVar(fileout, filetype);
                if(ctl == GFX_FRAME_DONE) ti_SetArchiveStatus(true, filehandle);
                ti_Close(filehandle);
                ti_RenameVar(filein, fileout, filetype);
                if(ctl == MAIN_FRAME_DONE){
                    uint8_t outctl = DISCONNECT;
                    ntwk_queue(&outctl, sizeof outctl);
                    ntwk_send();
                    gfx_End();
                    usb_Cleanup();
                    os_RunPrgm(fileout, NULL, 0, NULL);
                }
                else if(ctl == GFX_FRAME_DONE){
                    if(TrekGFX_init()){
                        gfx_InitModuleIcons();
                        gfx_VersionCheck();
                        ntwk_send_nodata(LOAD_SHIP);
                    }
                }
            }
            else {
                
                ti_Close(filehandle);
                ti_Delete(filein);
                if(ctl == MAIN_FRAME_DONE) {
                    gfx_ErrorClearBG("client download error", 20, 190);
                    srv_request_client(&filestream_hash);
                }
                else if(ctl == GFX_FRAME_DONE) {
                    gfx_ErrorClearBG("gfx download error", 20, 190);
                    srv_request_gfx(&filestream_hash);
                }
            }
            break;
        }
        case GFX_SKIP:
            gameflags.gfx_loaded = TrekGFX_init();
            if(gameflags.gfx_loaded){
                gfx_InitModuleIcons();
                gfx_VersionCheck();
                ntwk_send_nodata(LOAD_SHIP);
            }
            break;
        case MAIN_SKIP:
            gfx_TextClearBG("initiating secure session...", 20, 190, true);
            ntwk_send_nodata(REQ_SECURE_SESSION);
            break;
        default:
            gui_SetLog(LOG_ERROR, "unknown packet received");
    }
}


