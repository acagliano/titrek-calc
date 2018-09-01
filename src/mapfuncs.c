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

void map_MoveObjects(MapData_t* map){
    char i;
    for(i = 0; i < 20; i++){
        MapData_t *entity = &map[i];
        if(entity->entitytype){
            if(entity->mobile){
                proc_MoveEntity(&entity->position, entity->speed);
                if(!entity->entitystats.weapon.range--) entity->entitytype = 0;
            }
        }
    }
}
