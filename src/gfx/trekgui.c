// convpng v7.0
#include <stdint.h>
#include "trekgui.h"

#include <fileioc.h>
uint8_t *trekgui[29] = {
 (uint8_t*)0,
 (uint8_t*)13818,
 (uint8_t*)26968,
 (uint8_t*)39946,
 (uint8_t*)42078,
 (uint8_t*)44007,
 (uint8_t*)45940,
 (uint8_t*)47280,
 (uint8_t*)47369,
 (uint8_t*)47460,
 (uint8_t*)47564,
 (uint8_t*)47634,
 (uint8_t*)47727,
 (uint8_t*)47836,
 (uint8_t*)48020,
 (uint8_t*)48404,
 (uint8_t*)48654,
 (uint8_t*)48776,
 (uint8_t*)48896,
 (uint8_t*)49052,
 (uint8_t*)49160,
 (uint8_t*)49352,
 (uint8_t*)49484,
 (uint8_t*)49594,
 (uint8_t*)49692,
 (uint8_t*)49903,
 (uint8_t*)50129,
 (uint8_t*)50187,
 (uint8_t*)50291,
};

bool trekgui_init(void *decompressed_addr) {
    unsigned int data, i;

    data = (unsigned int)decompressed_addr - (unsigned int)trekgui[0];
    for (i = 0; i < trekgui_num; i++) {
        trekgui[i] += data;
    }
    return 1;
}
