#ifndef USB_H
#define USB_H

#include <stdint.h>

void init_usb(void);
struct pico_device *pico_eem_create(const char *name, uint8_t *mac);

#endif //USB_H
