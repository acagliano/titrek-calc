
#ifndef player_h
#define player_h

typedef struct {
    char username[24];
    char passwd[32];
} userinfo_t;

typedef struct {
    uint24_t credits;
    uint8_t faction;
    
} playersave_t;

#endif
