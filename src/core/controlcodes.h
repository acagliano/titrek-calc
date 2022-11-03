
#ifndef controlcodes_h
#define controlcodes_h

enum ControlCodes {
    // CONNECTION/ADMIN
    // 0 - 9
    CONNECT,            // 0
    DISCONNECT,         // 1
    RSA_KEY_FROM_SERV,     // 2
    SEND_AES_SECRET,   // 3
    LOGIN,            // 4
    // This is for the client not up to date response , See response codes. VERSION_* must be the second byte of packet
      
	// Ship Interfacing: 20 - 29
	LOAD_SHIP = 0x10,
	MODULE_INFO_REQUEST,            // 21
	MODULE_CONFIG,            // 22
	NEW_GAME_REQUEST,               // 23
	GET_ENGINE_MAXIMUMS,            // 24
	ENGINE_SETSPEED,                // 25
	
    // GAMEPLAY
    // Map Interfacing: 10 - 19
    FRAMEDATA_REQUEST = 0x20,
    SENSOR_DATA_REQUEST,        // 11
    PLAYER_MOVE,                // 12
    POSITION_REQUEST,           // 13
       
    // Map Streaming : 30 - 39
    RENDER_FBUF_OBJ = 30,
    RENDER_FBUF_DATA,
       
    // File Streaming
    GFX_REQ_UPDATE = 0xe0,
    GFX_FRAME_START,                // 0xe1
    GFX_FRAME_IN,                   // 0xe2
    GFX_FRAME_NEXT,                 // 0xe3
    GFX_FRAME_DONE,                 // 0xe4
    GFX_SKIP,                       // 0xe5
    
    MAIN_REQ_UPDATE,                // 0xe6
    MAIN_FRAME_START,               // 0xe7
    MAIN_FRAME_IN,                  // 0xe8
    MAIN_FRAME_NEXT,                // 0xe9
    MAIN_FRAME_DONE,                // 0xea
    MAIN_SKIP,                // 0xeb
    
    PRGMUPDATE = 90,
    GFXUPDATE,
    CACHE_SPRITE,
       
    // DEBUG: 0xf*
    BRIDGE_ERROR = 0xf0,
    PING = 0xfc,
    MESSAGE,
    DEBUG,
    SERVINFO
};

//returned from some routines
enum _connection_response {
    SUCCESS = 0,
    INVALID,
    DUPLICATE,
    MISSING,
    BANNED,
    UNVERIFIED,
    BAD_MESSAGE_CONTENT
};

enum _versioncheck_response {
    VERSION_OK,
    VERSION_OUTDATED,
    VERSION_ERROR
};

/*
    Module Update Notes
    MODULEUPDATE
*/
enum _module_state_change_response {
// for inbound (server->client)
    CHANGE_STATUS_FLAGS,     // edit status flags
    CHANGE_HEALTH,     // update saved health %
// for outbound (client->server)
    CHANGE_ONLINE_STATE = 10
};
// client may also send a MODULEUPDATE request when "purchasing" an upgrade.
// We may wind up storing upgrades that enhance existing modules (instead of be their own tech) in their own database, such that they may be requested by sending a MODULEUPDATE with the id.
// Server-side, these upgrades will likely be stored as Name, Attribute to upgrade, amount of upgrade, cost.


#endif
