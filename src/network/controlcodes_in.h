#ifndef control_in_h
#define control_in_h

enum ControlCodes_Inbound {
    REGISTER,
    LOGIN,
    DISCONNECT,
    RESP_PING = 0x000d,
    RESP_MESSAGE = 0x000e,
};

//returned from the registration routine
enum RegLogCodes {
    RL_SUCCESS,
    RL_INVALID,
    RL_DUPLICATE,
    RL_MISSING,
    RL_BANNED,
    RL_VERSION_MISMATCH
};

//returned from the login routine

//returned from the disconect routine
enum DisconnectError {
    DISC_SUCCESS
};


#endif
