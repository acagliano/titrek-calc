
#ifndef coords_h
#define coords_h

typedef struct {
    signed long x;
    signed long y;
    signed long z;
} coords_t;


typedef struct {
    signed long x;
    signed long y;
    signed long z;
} vector_t;
vector_t* coords_GetVector(coords_t* origin, coords_t* dest);


#endif
