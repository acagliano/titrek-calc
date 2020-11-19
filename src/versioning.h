#ifndef versioning_h
#define versioning_h

#include <stddef.h>
#include <graphx.h>
#include "gfx/TrekGFX.h"

extern uint8_t version[3];
extern char versionstr[12];
extern uint8_t gfx_version[2];
extern uint8_t gfx_reqd[2];
extern uint8_t gfx_custom[2];

void gfx_GetVersion(void);
uint8_t gfx_getCustomPackCount(void);
void gfx_VersionCheck(void);
char *strify_version(char *str, uint8_t *version);

#endif
