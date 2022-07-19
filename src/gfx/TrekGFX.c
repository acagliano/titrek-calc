#include "TrekGFX.h"
#include <fileioc.h>

#define TrekGFX_HEADER_SIZE 2

unsigned char *TrekGFX_appvar[31];

unsigned char TrekGFX_init(void)
{
    uint8_t appvar;
    unsigned int *table;
    void *base;
    unsigned char i;

    appvar = ti_Open("TrekGFX", "r");
    if (appvar == 0)
    {
        return 0;
    }

    table = base = (char*)ti_GetDataPtr(appvar) + TrekGFX_HEADER_SIZE;
    if (*table != 31)
    {
        ti_Close(appvar);
        return 0;
    }

    for (i = 0; i < 31; i++)
    {
        TrekGFX_appvar[i] = (void*)(*++table + (unsigned int)base);
    }

    ti_Close(appvar);

    return 1;
}

