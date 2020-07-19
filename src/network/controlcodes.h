#ifndef controlcodes_h
#define controlcodes_h

enum ControlCodes {
    // network handling
    REGISTER,       // attempt to create new account
    LOGIN,          // attempt to join existing account
    DISCONNECT,     // attempt to disconnect from the server
    // autoupdater
    PGRMUPDATE,     // data segment for client self-update
    GFXUPDATE,      // data segment for graphics update
    // load game data
    LOADGAME,       // receive necessary data to render GUI (ship_t)
    // no need for a SAVEGAME because everything is server-side anyway
    // in-game stuff
    MODULEINFO,     // request all module information (instead of abbreviated)
    MODULEUPDATE,   // used by server to send info to client. Also used by client at times.
    REQUESTCHUNK,       // request chunk framedata
    REQUESTENTITY,      // request entity framedata
    REQUESTSENSOR,  // Ask server for (1) position, (2) rotation matrix, (3) sensor data
    PING = 0xfc,
    MESSAGE = 0xfd,
    DEBUG = 0xfe,
    SERVINFO = 0xff
};

//returned from some routines
enum ResponseCodes {
    SUCCESS = 0,
    INVALID,
    DUPLICATE,
    MISSING,
    BANNED,
    VERSION_MISMATCH
};

/*
    Module Update Notes
    MODULEUPDATE
*/
enum ModuleUpdateCodes {
// for inbound (server->client)
    UPD_STATUS,     // update online/offline state
    UPD_HEALTH,     // update saved health %
    UPD_POWER    // update saved power %
    UPD_REASSIGN // change entire module (usually replacing with new tech)
}
// client may also send a MODULEUPDATE request when "purchasing" an upgrade.
// We may wind up storing upgrades that enhance existing modules (instead of be their own tech) in their own database, such that they may be requested by sending a MODULEUPDATE with the id.
// Server-side, these upgrades will likely be stored as Name, Attribute to upgrade, amount of upgrade, cost.


#endif
