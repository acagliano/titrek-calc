#ifndef controlcodes_h
#define controlcodes_h

enum ControlCodes {
    // CONNECTION/ADMIN
    // 0 - 9
    REGISTER,
    LOGIN,
    DISCONNECT,
    VERSION_MISMATCH,
    // This is for the client not up to date response , See response codes. VERSION_* must be the second byte of packet
       
    // GAMEPLAY
    // Map Interfacing: 10 - 19
    FRAMEDATA_REQUEST = 10,
    SENSOR_DATA_REQUEST,     // May add REQPOSITION if needed
    PLAYER_MOVE,
    POSITION_REQUEST,
       
    // Ship Interfacing: 20 - 29
    LOAD_SHIP = 20,
    MODULE_INFO_REQUEST,
    MODULE_STATE_CHANGE,
    NEW_GAME_REQUEST,
       
    // File Streaming: 90 - 91
    PRGMUPDATE = 90,
    GFXUPDATE,
       
    // DEBUG: 0xf*
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
    VERSION_ERROR,
    VERSION_OUTDATED,
    BAD_MESSAGE_CONTENT
};

/*
    Module Update Notes
    MODULEUPDATE
*/
enum _module_state_change_response {
// for inbound (server->client)
    CHANGE_STATUS,     // update online/offline state
    CHANGE_HEALTH,     // update saved health %
    CHANGE_POWER,    // update saved power %
    CHANGE_TECH // change entire module (usually replacing with new tech)
};
// client may also send a MODULEUPDATE request when "purchasing" an upgrade.
// We may wind up storing upgrades that enhance existing modules (instead of be their own tech) in their own database, such that they may be requested by sending a MODULEUPDATE with the id.
// Server-side, these upgrades will likely be stored as Name, Attribute to upgrade, amount of upgrade, cost.


#endif
