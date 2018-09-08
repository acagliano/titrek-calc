// convpng v6.8
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[23] = {
 (uint8_t*)0,
 (uint8_t*)69,
 (uint8_t*)116,
 (uint8_t*)289,
 (uint8_t*)468,
 (uint8_t*)622,
 (uint8_t*)672,
 (uint8_t*)710,
 (uint8_t*)796,
 (uint8_t*)2106,
 (uint8_t*)2164,
 (uint8_t*)2227,
 (uint8_t*)2265,
 (uint8_t*)2284,
 (uint8_t*)2306,
 (uint8_t*)2326,
 (uint8_t*)2351,
 (uint8_t*)2425,
 (uint8_t*)2562,
 (uint8_t*)2588,
 (uint8_t*)2644,
 (uint8_t*)2675,
 (uint8_t*)2792,
};

bool trekgui_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("trekgui", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
