
#ifndef controlcodes_h
#define controlcodes_h

enum TypeCodes {
    REGISTER,
    LOGIN,
    DISCONNECT,
    RESP_PING = 0x000d,
    RESP_MESSAGE = 0x000e,
    RESP_DEBUG = 0x000f,
    RESP_SERVINFO = 0x00ff
};

//returned from the registration routine
enum RegisterCodes {
    REG_SUCCESS,
    REG_INVALID,
    REG_DUPLICATE,
    REG_MISSING
};

//returned from the login routine
enum LoginError {
    LOG_SUCCESS,
    LOG_INVALID,
    LOG_DUPLICATE,
    LOG_MISSING
};

//returned from the disconect routine
enum DisconnectError {
    DISC_SUCCESS
};

#endif
