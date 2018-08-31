// convpng v6.8
#include <stdint.h>
#include "trekvfx.h"

#include <fileioc.h>
uint8_t *trekvfx[3] = {
 (uint8_t*)0,
 (uint8_t*)102,
 (uint8_t*)219,
};

bool trekvfx_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("trekvfx", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)trekvfx[0];
    for (i = 0; i < trekvfx_num; i++) {
        trekvfx[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
