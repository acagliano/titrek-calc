
#ifndef particles_h
#define particles_h

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PARTICLES 10

enum ParticleTypes {
    NONE,
    CRACKED_SCREEN,
    SPARKS
};

typedef struct {
    uint8_t type;
    int24_t timeout;
    uint24_t x;
    uint8_t y;
} particles_t;

void gfx_RenderParticles(particles_t* part, uint8_t ct);
bool gfx_SetParticle(particles_t* part, uint8_t ct, uint8_t type);
void gfx_ClearParticles(particles_t* part, uint8_t ct);

#endif
