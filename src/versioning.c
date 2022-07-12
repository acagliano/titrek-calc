#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <fileioc.h>
#include "gfx/TrekGFX.h"
#include "flags.h"
#define TrekGFX_HEADER_SIZE 2

uint8_t version[] = {0, 0, 110};
char versionstr[16] = {0};
uint8_t gfx_version[2] = {0};
uint8_t gfx_reqd[2] = {1, 5};
uint8_t gfx_custom[2] = {0xff, 0xff};

void gfx_GetVersion(void){
    ti_var_t appv;
    if(appv = ti_Open("TrekGFX", "r")){
        ti_Read(&gfx_version, 2, 1, appv);
        ti_Close(appv);
    }
}

uint8_t gfx_getCustomPackCount(void){
    ti_var_t appvar;
    unsigned int *table;
    void *base;
    if(!(appvar = ti_Open("TrekGFX", "r"))) return 0;
    table = base = (char*)ti_GetDataPtr(appvar) + TrekGFX_HEADER_SIZE;
    ti_Close(appvar);
    return *table;
}

void gfx_VersionCheck(void){
	gfx_GetVersion();
    if((gfx_version[0] == 0xff) && (gfx_version[1] == 0xff)) {
        gameflags.gfx_custom = true;
        if(gfx_getCustomPackCount() == TrekGFX_entries_num) return;
        gameflags.gfx_error = true;
        return;
    }
    if((gfx_version[0] >= gfx_reqd[0]) && (gfx_version[1] >= gfx_reqd[1]))
        return;
    gameflags.gfx_error = true;
}

char *strify_version(char *str, uint8_t *version){
  char buf[12];
  int len,inx,vi;
  len=inx=0;
  for (vi=0; vi<3; vi++){
    sprintf(&buf,"%d\.",version[vi]);
    len=strlen(&buf);
    memcpy(str+inx,&buf,len);
    inx+=len;
  }
  str[inx-1]=0;
  return str;
}
