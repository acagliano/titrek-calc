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
 (uint8_t*)36118,
 (uint8_t*)36222,
 (uint8_t*)36292,
 (uint8_t*)36385,
 (uint8_t*)36494,
 (uint8_t*)36605,
 (uint8_t*)36789,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;

    data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }
}
