
#ifndef controlcodes_h
#define controlcodes_h

enum ControlCodes {
    SERV_RESPONSE,
    SERV_REGISTER,
    SERV_DISCONNECT,
    SERV_PING = 0x0d,
    SERV_MESSAGE = 0x0e,
    SERV_DEBUG = 0x0f,
    SERV_SERVINFO = 0xff
};

enum ResponseCodes {
    RESP_REGISTER = 0x0001,
    RESP_LOGIN,
    RESP_DISCONNECT,
    RESP_PING = 0x000d,
    RESP_MESSAGE = 0x000e,
    RESP_DEBUG = 0x000f,
    RESP_SERVINFO = 0x00ff
};

enum InputCodes {
    IO_SUCCESS,
    IO_INVALID,
    IO_DUPLICATE,
    IO_MISSING
};

#returned from the registration routine
enum RegisterCodes {
    REG_SUCCESS = 0x000100,
    REG_INVALID,
    REG_DUPLICATE,
    REG_MISSING
};

#returned from the login routine
enum LoginError {
    LOG_SUCCESS = 0x000200,
    LOG_INVALID,
    LOG_DUPLICATE,
    LOG_MISSING
};

#returned from the disconect routine
enum DisconnectError {
    DISC_SUCCESS = 0x000300
};

#endif
