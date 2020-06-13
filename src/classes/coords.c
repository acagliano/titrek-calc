
#include "ships.h"
#include "coords.h"


vector_t* coords_GetVector(coords_t* origin, coords_t* dest){
    vector_t vector;
    signed long x = origin->x - dest->x;
    signed long y = origin->y - dest->y;
    signed long z = origin->z - dest->z;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    return &vector;
}
