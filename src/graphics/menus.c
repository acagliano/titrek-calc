

#define CEMU_CONSOLE ((char*)0xFB0000)

void MainMenu(void) {
    uint8_t opt = 0;
    while(1){
        opt = gfx_RenderSplash(splash, playgame_return);
        if(opt == OPT_PLAY) ServerSelect();
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
            while(kb_IsDown(kb_KeyClear)) kb_Scan();
            opt = 0;
        }
        if(opt == OPT_SETTINGS){
            uint24_t start_x = 20, start_y = 5, i, sel = 0;
            bool firstrender = true;
            sk_key_t key;
            do {
                key = getKey();
                if(key == sk_Down) sel = sel + (sel < (NUM_SETTINGS-1));
                if(key == sk_Up) sel = sel - (sel > 0);
                if(key == sk_Left) gfx_AlterSettingOpt(sel, -1);
                if(key == sk_Right) gfx_AlterSettingOpt(sel, 1);
                if(key || firstrender){
                    gfx_ZeroScreen();
                    for(i = 0; i < NUM_SETTINGS; i++)
                        gfx_RenderSettingOpt(i, sel, start_x, i * 16 + start_y);
                    gfx_BlitBuffer();
                    firstrender = false;
                }
    
            } while (key != sk_Clear);
            while(kb_AnyKey()) kb_Scan();
            opt = 0;
        }
    }
}


void ServerSelect(void){
    uint8_t i, selected = 0;
    bool render = true;
    sk_key_t key = 0;
    char *var_name;
    void *vat_ptr = NULL;
    char *filenames = NULL;
    size_t filect = 0, offset=0;
    if(gameflags.gfx_error) return;
    
    // parse VAT for files containing TrekKey string at beginning.
    while ((var_name = ti_Detect(&vat_ptr, "TrekKey"))){
        filenames = realloc(filenames, sizeof(filenames)+9);
        strncpy(9*filect+filenames, var_name, 9);
        filect++;
    }
    if(filenames==NULL) return;
    

    do {
        char* hostinfo;
        key = getKey();
        
        if(key == sk_Clear) break;
        if((key == sk_Left) && (offset>0)) {offset--; render=true;}
        if((key == sk_Right) && (offset<(filect-1))) {offset++; render=true;}
        
        if(key == sk_Enter){
            ti_var_t tfp = ti_Open(9*offset+filenames, "r");
            if(!tfp) return;
            strcpy(serverinfo.appvname, 9*offset+filenames);
            strcpy(serverinfo.hostname, ti_GetDataPtr(tfp)+7);
            ti_Close(tfp);
            playgame_return = PlayGame();
            break;
        }
        
        if(render){
            window_data_t win = {3, 260, 20, 90, 195, 2, 181};
            ti_var_t tfp = ti_Open(9*offset+filenames, "r");
            gfx_ZeroScreen();
            
            // main GUI elements
            gfx_RenderMenuTitle("Server Info", 3, 5);
            gfx_RenderWindow(&win);
            gfx_SetTextFGColor(255);
            gfx_PrintStringXY("[Enter] Connect to selected host", 5, 120);
            if(offset>0) gfx_ColoredText("[Left] Select previous server", 5, 130, 255);
            else gfx_ColoredText("[Left] Select previous server", 5, 130, 74);
            if(offset<(filect-1)) gfx_ColoredText("[Right] Select next server", 5, 140, 255);
            else gfx_ColoredText("[Right] Select next server", 5, 140, 74);
            gfx_SetTextFGColor(0);
            if(tfp) {
                // if appv opened
                size_t filesize = ti_GetSize(tfp);
                size_t keylen = filesize - strlen(ti_GetDataPtr(tfp)+7);
                keylen -= 8; keylen *= 8;
                gfx_PrintStringXY("Keyfile: ", 10, 35);
                gfx_PrintString(9*offset+filenames);
                gfx_RLETSprite(log_server, 10, 50);
                gfx_RLETSprite(icon_security, 10, 65);
                gfx_PrintStringXY(ti_GetDataPtr(tfp)+7, 25, 50);
                gfx_SetTextXY(25, 65);
                gfx_PrintUInt(keylen, num_GetLength(keylen));
                gfx_PrintString("-bit session key");
                gfx_PrintStringXY("RSA-<=2048 cipher, key exch", 25, 75);
                gfx_PrintStringXY("AES-256 cipher, login", 25, 85);
                
                ti_Close(tfp);
            }
            else {
                gfx_PrintStringXY("Error opening file", 10, 25);
            }
            gfx_BlitBuffer();
            render = false;
        }
        
        ntwk_process();
    } while (1);
}
