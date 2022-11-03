#include "uxassets.h"
#include <fileioc.h>

#define uxassets_HEADER_SIZE 0

unsigned char *uxassets_appvar[19];

unsigned char uxassets_init(void)
{
    uint8_t appvar;
    unsigned int *table;
    void *base;
    unsigned char i;

    appvar = ti_Open("uxassets", "r");
    if (appvar == 0)
    {
        return 0;
    }

    table = base = (char*)ti_GetDataPtr(appvar) + uxassets_HEADER_SIZE;
    if (*table != 19)
    {
        ti_Close(appvar);
        return 0;
    }

    for (i = 0; i < 19; i++)
    {
        uxassets_appvar[i] = (void*)(*++table + (unsigned int)base);
    }

    ti_Close(appvar);

    return 1;
}

