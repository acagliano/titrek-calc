
#include <tice.h>
#include <stdbool.h>
#include <string.h>

#include <graphx.h>
#include <usbdrvce.h>
#include <hashlib.h>
#include <keypadc.h>
#include <fileioc.h>
#include <compression.h>

#include "network.h"
#include "gameloop.h"
#include "controlcodes.h"
#include "../graphics/text.h"
#include "../graphics/console.h"

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
    
    if(settings.debug)
        #define DBG_MSG_EXPECTED_LEN 50
        static char log_msg[DBG_MSG_EXPECTED_LEN];
        sprintf(log_msg, "Packet In, Type: %u Size: %u", ctl, buff_size);
        console_write(ENTRY_DEBUG_MSG, log_msg);
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
                uint8_t out_ctl_code = RSA_SEND_SESSION_KEY;
                
                console_write(ENTRY_NORMAL, "Exchanging AES secret with server");
                csrand_fill(aes_key, AES_KEYLEN);
                rsa_encrypt(aes_key, AES_KEYLEN, msg, key, keylen, SHA256);
                
                ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
                ntwk_queue(&msg, keylen);
                ntwk_send();
                break;
            }
        case RSA_SEND_SESSION_KEY:
        {
            #define LOGIN_TOKEN_SIZE 128
            #define PPT_LEN (LOGIN_TOKEN_SIZE+AES_BLOCKSIZE)
            #define KF_OFFSET_HOSTNAME 7
            aes_ctx ctx;
            uint8_t iv[AES_BLOCKSIZE];
            uint8_t ct[PPT_LEN];
            ti_var_t tfp = ti_Open(hostinfo.fname, "r");
            uint8_t ctl_out_code = LOGIN;
            if(!tfp) {
                console_write(ENTRY_ERROR_MSG, "Error opening keyfile");
                break;
            };
    
            console_write(ENTRY_NORMAL, "Securely logging you in");
            
            aes_init(aes_key, &ctx, 32);         // load secret key
            csrand_fill(iv, AES_BLOCKSIZE);     // get IV
    
            // retrieve pointer to hostname (appv_start + 7)
            char* hostname = ti_GetDataPtr(tfp) + KF_OFFSET_HOSTNAME;
            // pointer to start of key = hostname + strlen(hostname) + 1 (null term)
            char* keydata = hostname + strlen(hostname) + 1;
    
            // close filestream
            ti_Close(tfp);
            
            // Encrypt the login token with AES-256
            if(aes_encrypt(keydata, LOGIN_TOKEN_SIZE, ct, &ctx, iv, AES_MODE_CBC, SCHM_DEFAULT) != AES_OK){
                console_write(ENTRY_ERROR_MSG, "AES cipher error");
                break;
            }

            ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
            ntwk_queue(iv, AES_BLOCKSIZE);
            ntwk_queue(ct, PPT_LEN);
            ntwk_send();
    
            // Zero out key schedule, key used, and IV
            break;
        }
        case CONNECT:
        {
            tick_loop_mode = SERVER_CONNECTED;
            
            // hash current client binary to check with server
            uint8_t digest[SHA256_DIGEST_SIZE];
            uint8_t ctl_out_code = MAIN_REQ_UPDATE;
            console_write(ENTRY_NORMAL, "Validating client hash with server");
            hash_init(&filestream_hash, SHA256);
            if((filehandle = ti_OpenVar("TITREK", "r", OS_TYPE_PROT_PRGM))){
                hash_update(&filestream_hash, ti_GetDataPtr(filehandle), ti_GetSize(filehandle));
                ti_Close(filehandle);
            }
            hash_final(&filestream_hash, digest);
            
            ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
            ntwk_queue(digest, SHA256_DIGEST_SIZE);
            ntwk_send();
            break;
        }
        case DISCONNECT:
            tick_loop_mode = NO_CONNECTION;
            network_error = NTWK_PEER_DISCONNECT;
            break;
        case BRIDGE_ERROR:
            network_error = NTWK_BRIDGE_ERR;
            break;
        case LOGIN:
            if(response == SUCCESS) {
                tick_loop_mode = USER_LOGGED_IN;
                srv_request_gfx(&filestream_hash);        // see lcars/gui.c
            }
            else if(response==INVALID){
                console_write(ENTRY_SERVER_MSG, "Invalid session token. Unable to log in.");
                network_error = NTWK_AUTH_ERR;
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
            console_write(ENTRY_NORMAL, data);
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
            ntwk_DisableTimeout();
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
            uint8_t filetype = (ctl==MAIN_FRAME_DONE) ? OS_TYPE_PROT_PRGM : TI_APPVAR_TYPE;
            hash_init(&filestream_hash, SHA256);
            memcpy(&file_dl_size, data, sizeof(size_t));
            file_bytes_written = 0;
            if(!(filehandle = ti_OpenVar(file, "w", filetype))) break;
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
                uint8_t filetype = (ctl==MAIN_FRAME_DONE) ? OS_TYPE_PROT_PRGM : TI_APPVAR_TYPE;
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


