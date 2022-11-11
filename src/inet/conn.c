#include <string.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>
#include "inet.h"
#include "conn.h"
#include "../gamestate.h"
#include "../io/frames.h"

char hostinfo[64] = {0};
char username[32] = {0};
#define TOKEN_LEN	64
uint8_t token[TOKEN_LEN] = {0};

void bridge_ping(void);
void bridge_connect(const char *metafile);
void bridge_disconnect(void);
void server_ping(void);

conn_t conn = {
	CONN_OK,
	{bridge_ping, bridge_connect, bridge_disconnect},
	{server_ping, inet_send_packet}
};
bridge_funcs *bridge = &conn.bridge;
server_funcs *server = &conn.server;

void bridge_ping(void){ inet_send_packet(0, PING_BRIDGE); }

void bridge_connect(const char *metafile) {
	ti_var_t fp = ti_Open(metafile, "r");
	if(!fp) { conn.error = METAFILE_IO_ERROR; return; }
	uint8_t device_type = (gamestate.inet.flags>>INET_DEVICE) & 3;
	char *hptr = ti_GetDataPtr(fp) + strlen("TrekIdentity");
	const char *scrn_title = "Connection Status";
	strncpy(hostinfo, hptr, strlen(hptr));
	hptr += strlen(hptr) + 1;
	strncpy(username, hptr, strlen(hptr));
	hptr += strlen(hptr) + 1;
	memcpy(token, hptr, TOKEN_LEN);
	ti_Close(fp);
	
	//frame_render_headerbar(scrn_title);
	inet_send_packet(1, CONNECT, hostinfo, strlen(hostinfo));
}

void bridge_disconnect(void){ inet_send_packet(0, DISCONNECT); }

void server_ping(void) { inet_send_packet(0, PING_SERVER); }
