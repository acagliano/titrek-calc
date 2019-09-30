// convpng v7.0
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[25] = {
 (uint8_t*)0,
 (uint8_t*)13150,
 (uint8_t*)26128,
 (uint8_t*)29497,
 (uint8_t*)32418,
 (uint8_t*)34885,
 (uint8_t*)35938,
 (uint8_t*)36027,
 (uint8_t*)36118,
 (uint8_t*)36222,
 (uint8_t*)36292,
 (uint8_t*)36385,
 (uint8_t*)36494,
 (uint8_t*)36605,
 (uint8_t*)36789,
 (uint8_t*)36943,
 (uint8_t*)37049,
 (uint8_t*)37239,
 (uint8_t*)37369,
 (uint8_t*)37477,
 (uint8_t*)37573,
 (uint8_t*)37782,
 (uint8_t*)38006,
 (uint8_t*)38062,
 (uint8_t*)38164,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;

    data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }

    return (bool)appvar;
}
