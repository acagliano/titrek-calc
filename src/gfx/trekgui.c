// convpng v7.0
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[1] = {
 (uint8_t*)0,
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
