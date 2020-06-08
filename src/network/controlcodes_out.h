
#ifndef control_out_h
#define control_out_h

enum ControlCodes_Outbound {
    REGISTER,
    LOGIN,
    DISCONNECT,
    PLAYER_MOV,
    CHUNK_REQ,
    SEND_PING = 0x00d,
    SEND_MESSAGE = 0x00e,
    DEBUG_ENABLE = 0x0ff
}


#endif
