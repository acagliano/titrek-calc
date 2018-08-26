// convpng v6.8
#include <stdint.h>
#include "trekicon.h"

#include <fileioc.h>
uint8_t *trekicon[9] = {
 (uint8_t*)0,
 (uint8_t*)69,
 (uint8_t*)116,
 (uint8_t*)289,
 (uint8_t*)468,
 (uint8_t*)622,
 (uint8_t*)672,
 (uint8_t*)710,
 (uint8_t*)796,
};

bool trekicon_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("trekicon", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)trekicon[0];
    for (i = 0; i < trekicon_num; i++) {
        trekicon[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
