// convpng v6.8
#include <stdint.h>
#include "TrekVFX.h"

#include <fileioc.h>
uint8_t *TrekVFX[1] = {
 (uint8_t*)0,
};

bool TrekVFX_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("TrekVFX", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)TrekVFX[0];
    for (i = 0; i < TrekVFX_num; i++) {
        TrekVFX[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
