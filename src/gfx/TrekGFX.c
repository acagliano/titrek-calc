#include "TrekGFX.h"
#include <fileioc.h>

#define TrekGFX_HEADER_SIZE 2

unsigned char *TrekGFX_appvar[30];

unsigned char TrekGFX_init(void)
{
    ti_var_t appvar;
    unsigned int *table;
    void *base;
    unsigned char i;

    ti_CloseAll();

    appvar = ti_Open("TrekGFX", "r");
    if (appvar == 0)
    {
        return 0;
    }

    table = base = (char*)ti_GetDataPtr(appvar) + TrekGFX_HEADER_SIZE;
    if (*table != 30)
    {
        return 0;
    }

    for (i = 0; i < 30; i++)
    {
        TrekGFX_appvar[i] = (void*)(*++table + (unsigned int)base);
    }

    ti_CloseAll();

    return 1;
}

