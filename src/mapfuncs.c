#include <stddef.h>
#include "datatypes/mapdata.h"


char map_LocateSlot(MapData_t* map){
    char i;
    for(i = 0; i < 19; i++){
        MapData_t *slot = &map[i];
        if(!slot->entitytype) return i;
    }
    return -1;
}

void map_PopulateSlot(MapData_t* slot, char type){
    MapData_t *object = slot;
}
