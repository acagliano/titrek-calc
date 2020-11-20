
#ifndef engine_h
#define engine_h

#define gfx_ResetClip() gfx_SetClipRegion((0),(0),(320),(240))

typedef struct {
    int8_t x,y;
    uint8_t radius;
    uint8_t bodycolor; //fill color
    uint8_t secondarycolor; //ignore this for now
    uint8_t atmospherecolor; //circle line color
    uint8_t randomseed;
    uint8_t unused; //unused for now
} frame_body_t;

typedef struct {
    int8_t x,y;
    uint8_t radius;
    uint8_t color;
} sensor_body_t;

typedef struct {
    uint8_t numbodies;
    frame_body_t bodies[1];
} body_packet_t;

typedef struct {
    uint8_t numbodies;
    sensor_body_t bodies[1];
} sensor_packet_t;


void renderFrame(body_packet_t* frame);



#endif
