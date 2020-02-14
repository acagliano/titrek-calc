// convpng v7.2
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[28] = {
 (uint8_t*)0,
 (uint8_t*)13150,
 (uint8_t*)26128,
 (uint8_t*)28260,
 (uint8_t*)30189,
 (uint8_t*)32122,
 (uint8_t*)33462,
 (uint8_t*)33551,
 (uint8_t*)33642,
 (uint8_t*)33746,
 (uint8_t*)33816,
 (uint8_t*)33909,
 (uint8_t*)34018,
 (uint8_t*)34202,
 (uint8_t*)34586,
 (uint8_t*)34836,
 (uint8_t*)34958,
 (uint8_t*)35078,
 (uint8_t*)35234,
 (uint8_t*)35342,
 (uint8_t*)35534,
 (uint8_t*)35666,
 (uint8_t*)35776,
 (uint8_t*)35874,
 (uint8_t*)36085,
 (uint8_t*)36311,
 (uint8_t*)36369,
 (uint8_t*)36473,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;

    data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }

    return true;
}
