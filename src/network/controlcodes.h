#ifndef controlcodes_h
#define controlcodes_h

enum ControlCodes {
    REGISTER,
    LOGIN,
    DISCONNECT,
    PGRMUPDATE,
    GFXUPDATE,
    SHIPDATAREQ,
    MODULEINFO,
    MOVESHIP,
    REQCHUNK,
    REQENTITY,
    REQSENSDATA, // May add REQPOSITION if needed
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


#endif
