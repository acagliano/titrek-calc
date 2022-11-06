/*
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
#include "settings.h"
#include "ship.h"

#include "../graphics/text.h"
#include "../graphics/console.h"
#include "../graphics/lcars-ui/components.h"

#define SHA256_DIGEST_SIZE SHA256_DIGEST_LEN

uint8_t aes_hmac_key[AES_KEYLEN + 16] = {0};

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

void versioncheck_hash_file(const char* file, uint8_t type, uint8_t* digest){
    ti_var_t tfp;
    hash_ctx ctx;
    hash_init(&ctx, SHA256);
    if((tfp = ti_OpenVar(file, "r", type))){
        hash_update(&ctx, ti_GetDataPtr(tfp), ti_GetSize(tfp));
        ti_Close(tfp);
    }
    hash_final(&ctx, digest);
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
    
    if(settings.debug_mode){
        #define DBG_MSG_EXPECTED_LEN 50
        static char log_msg[DBG_MSG_EXPECTED_LEN];
        sprintf(log_msg, "Packet In, Type: %u Size: %u", ctl, buff_size);
        console_write(ENTRY_DEBUG_MSG, log_msg);
    }
    
    switch(ctl){
        case RSA_KEY_FROM_SERV:
            {
                struct {
                    size_t kl;
                    uint8_t k[1];
                } *p = (void*)data;
                size_t keylen = p->kl;
                uint8_t* key = p->k;
                uint8_t msg[256] = {0};
                uint8_t out_ctl_code = SEND_AES_SECRET;
                
                console_write(ENTRY_NORMAL, "Negotiating AES key.");
                csrand_fill(aes_hmac_key, sizeof aes_hmac_key);
                rsa_encrypt(aes_hmac_key, sizeof aes_hmac_key, msg, key, keylen, SHA256);
                
                ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
                ntwk_queue(&msg, keylen);
                ntwk_send();
                break;
            }
        case SEND_AES_SECRET:
        {
            #define KF_OFFSET_HOSTNAME 7
            aes_ctx ctx;
			hmac_ctx hmac;
            uint8_t iv[AES_IVSIZE] = {0};
			uint8_t hmac_digest[SHA256_DIGEST_SIZE];
            ti_var_t tfp = ti_Open(hostinfo.fname, "r");
            uint8_t ctl_out_code = LOGIN;
            if(!tfp) {
                console_write(ENTRY_ERROR_MSG, "Error opening keyfile.");
                break;
            };
            char* fstart = ti_GetDataPtr(tfp);
            size_t fsize = ti_GetSize(tfp);
            size_t encsize = aes_outsize(fsize);
            ti_Close(tfp);
            uint8_t *encbuf = malloc(encsize);
            if(!encbuf){
                console_write(ENTRY_ERROR_MSG, "malloc error.");
                break;
            }
            console_write(ENTRY_NORMAL, "Securely logging you in.");
			
			csrand_fill(iv, AES_IVSIZE);
            aes_init(&ctx, aes_hmac_key, 32, iv, AES_MODE_CBC);         // load secret key
            
            // queue IV to the send buffer before using, encrypt will destroy it
    
            // Encrypt the login token with AES-256
            if(aes_encrypt(&ctx, fstart, fsize, encbuf) != AES_OK){
                console_write(ENTRY_ERROR_MSG, "AES cipher error.");
                break;
            }
			
			hmac_init(&hmac, &aes_hmac_key[32], 16, SHA256);
			hmac_update(&hmac, iv, AES_IVSIZE);
			hmac_update(&hmac, encbuf, encsize);
			hmac_final(&hmac, hmac_digest);

            ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
            ntwk_queue(iv, AES_IVSIZE);
            ntwk_queue(encbuf, encsize);
			ntwk_queue(hmac_digest, SHA256_DIGEST_SIZE);
            ntwk_send();
            
            free(encbuf);
    
            // Zero out key schedule, key used, and IV
            break;
        }
        case CONNECT:
        {
            tick_loop_mode = SERVER_UP;
            
            // hash current client binary to check with server
            uint8_t digest[SHA256_DIGEST_SIZE];
            uint8_t ctl_out_code = MAIN_REQ_UPDATE;
            console_write(ENTRY_NORMAL, "Validating client hash.");
            versioncheck_hash_file(main_program, OS_TYPE_PROT_PRGM, digest);
            
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
                
                // hash current graphics binary to check with server
                uint8_t digest[SHA256_DIGEST_SIZE];
                uint8_t ctl_out_code = GFX_REQ_UPDATE;
                console_write(ENTRY_NORMAL, "Validating graphics hash.");
                versioncheck_hash_file(main_gfx, OS_TYPE_APPVAR, digest);
                
                ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
                ntwk_queue(digest, SHA256_DIGEST_SIZE);
                ntwk_send();
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
            memcpy(ship_systems, data, buff_size-1);
            render_frame = true;
            break;
        case MODULE_INFO_REQUEST: /*
            {
                struct {
                    uint8_t slot;
                    moduleinfo_t info;
                } *packet = (void*)data;
                memcpy(&ModuleInfo, &packet->info, sizeof(ModuleInfo));
            }
            break;
        case MODULE_CONFIG:
        /*
            {
                struct {
                    uint8_t slot;
                    module_t newdata;
                } *packet = (void*)data;
                module_t* thismodule = &Ship.system[packet->slot];
                memcpy(thismodule, &packet->newdata, sizeof(module_t));
            }
            
            break;
        case ENGINE_SETSPEED: /*
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
        case GET_ENGINE_MAXIMUMS: /*
            memcpy(&engine_ref.engine[0], data, sizeof(engine_ref_t)-1);
            engine_ref.loaded = true;
            break;
        case MAIN_FRAME_START:
        case GFX_FRAME_START:               // 91
        {
            const char* file = (ctl==MAIN_FRAME_START) ? temp_program : temp_gfx;
            uint8_t out_ctl_code = (ctl==MAIN_FRAME_START) ? MAIN_FRAME_NEXT : GFX_FRAME_NEXT;
            uint8_t filetype = (ctl==MAIN_FRAME_START) ? OS_TYPE_PROT_PRGM : OS_TYPE_APPVAR;
            hash_init(&filestream_hash, SHA256);
            memcpy(&file_dl_size, data, sizeof(size_t));
            file_bytes_written = 0;
            if(!(filehandle = ti_OpenVar(file, "w", filetype))) break;
            lcars_SetBarColors(24, 23, 74);
            ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
            ntwk_send();
            break;
        }
        case MAIN_FRAME_IN:
        case GFX_FRAME_IN:                   // 92
        {
            char msg[DBG_MSG_EXPECTED_LEN] = {0};
            char fname[9] = {0};
            uint8_t out_ctl_code = (ctl==MAIN_FRAME_IN) ? MAIN_FRAME_NEXT : GFX_FRAME_NEXT;
            if(!filehandle) break;
            ti_GetName(fname, filehandle);
            if(ti_Write(data, buff_size-1, 1, filehandle))
                file_bytes_written += buff_size-1;
            hash_update(&filestream_hash, data, buff_size-1);
            sprintf(msg, "%s download", fname);
            lcars_DrawLoadBar(msg, 5, 200, 310, 5, 100*file_bytes_written/file_dl_size);
            //console_write(ENTRY_NORMAL, msg);
            ntwk_queue(&out_ctl_code, sizeof out_ctl_code);
            ntwk_send();
            break;
        }
        case MAIN_FRAME_DONE:
        case GFX_FRAME_DONE:        // 94
        {
            const char* filein = (ctl==MAIN_FRAME_DONE) ? temp_program : temp_gfx;
            const char* fileout = (ctl==MAIN_FRAME_DONE) ? main_program : main_gfx;
            char fname[9] = {0};
            if(!filehandle) break;
            ti_GetName(fname, filehandle);
            uint8_t digest[SHA256_DIGEST_SIZE];
            hash_final(&filestream_hash, digest);
            if(digest_compare(digest, data, SHA256_DIGEST_SIZE)){
                uint8_t filetype = (ctl==MAIN_FRAME_DONE) ? OS_TYPE_PROT_PRGM : OS_TYPE_APPVAR;
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
                else if(ctl == GFX_FRAME_DONE) ship_get_info();
            }
            else {
                char msg[DBG_MSG_EXPECTED_LEN] = {0};
                uint8_t ctl_out_code = (ctl == MAIN_FRAME_DONE) ? MAIN_REQ_UPDATE : GFX_REQ_UPDATE;
                uint8_t filetype = (ctl == MAIN_FRAME_DONE) ? OS_TYPE_PROT_PRGM : OS_TYPE_APPVAR;
                ti_Close(filehandle);
                ti_DeleteVar(filein, filetype);
                sprintf(msg, "%s download error, retrying download", fname);
                console_write(ENTRY_ERROR_MSG, msg);
                versioncheck_hash_file(fname, filetype, digest);
                
                ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
                ntwk_queue(digest, SHA256_DIGEST_SIZE);
                ntwk_send();
            }
            break;
        }
        case GFX_SKIP:
            ship_get_info();
            break;
        case MAIN_SKIP:
            {
                uint8_t ctl_out_code = REQ_SECURE_SESSION;
                ntwk_queue(&ctl_out_code, sizeof ctl_out_code);
                ntwk_send();
                break;
            }
        default:
        {
            char msg[DBG_MSG_EXPECTED_LEN] = {0};
            sprintf(msg, "unknown packet received, type %u", ctl);
            console_write(ENTRY_ERROR_MSG, msg);
        }
    }
}

*/
