// convpng v7.0
#include <stdint.h>
#include <compression.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[8] = {
 (uint8_t*)0,
 (uint8_t*)76802,
 (uint8_t*)76925,
 (uint8_t*)77048,
 (uint8_t*)77171,
 (uint8_t*)77294,
 (uint8_t*)77417,
 (uint8_t*)77540,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;
    ti_var_t appvar;
    ti_CloseAll();
    appvar = ti_Open("trekgui", "r");
    zx7_Decompress(decompressed_addr, ti_GetDataPtr(appvar));
   // data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += (unsigned int)decompressed_addr;
    }

    return (bool)appvar;
}
