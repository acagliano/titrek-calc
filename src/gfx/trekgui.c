// convpng v7.0
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[15] = {
 (uint8_t*)0,
 (uint8_t*)13150,
 (uint8_t*)26128,
 (uint8_t*)29497,
 (uint8_t*)32418,
 (uint8_t*)34885,
 (uint8_t*)35938,
 (uint8_t*)36027,
 (uint8_t*)36154,
 (uint8_t*)36281,
 (uint8_t*)36372,
 (uint8_t*)36476,
 (uint8_t*)36546,
 (uint8_t*)36639,
 (uint8_t*)36748,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;

    data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }

    return true;
}
