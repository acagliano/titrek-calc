
#ifndef components_h
#define components_h

#include <stdint.h>

void lcars_SetBarColors(
    uint8_t border_color,
    uint8_t active_color,
    uint8_t idle_color);
    
void lcars_DrawLoadBar(
    const char* title,
    uint24_t xstart,
    uint8_t ystart,
    uint24_t max_width,
    uint8_t bar_height,
    uint8_t percent);

void lcars_DrawBar(
    uint24_t xstart,
    uint8_t ystart,
    uint24_t max_width,
    uint8_t bar_height,
    uint8_t percent);


#endif
