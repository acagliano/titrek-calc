#ifndef conn_h
#define conn_h

typedef enum {
	CONN_OK,
	BRIDGE_NO_RESPONSE,
	METAFILE_IO_ERROR,
	DEVICE_ERROR,
	SERVER_NO_RESPONSE,
} conn_error_t;

typedef struct _bridge_funcs {
	void (*ping)();
	void (*connect_to_server)();
	void (*disconnect_from_server)();
} bridge_funcs;
typedef struct _server_funcs {
	void (*ping)();
	void (*send_packet)();
} server_funcs;
typedef struct _conn {
	uint8_t error;
	bridge_funcs bridge;
	server_funcs server;
} conn_t;
extern conn_t conn;
extern bridge_funcs *bridge;
extern server_funcs *server;

#define TOKEN_LEN			64
#define HOSTNAME_MAX_LEN	64
#define USERNAME_MAX_LEN	16
extern uint8_t token[TOKEN_LEN];
extern uint8_t hostinfo[HOSTNAME_MAX_LEN+1];
extern uint8_t username[USERNAME_MAX_LEN+1];

#endif
