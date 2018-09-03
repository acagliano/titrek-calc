#include <stddef.h>
#include "datatypes/mapdata.h"
#include "processes.h"
#include <string.h>


char map_LocateSlot(MapData_t* map){
    char i;
    for(i = 0; i < 19; i++){
        MapData_t *slot = &map[i];
        if(!slot->entitytype) return i;
    }
    return -1;
}

void map_MoveObjects(MapData_t* map, char tick){
    char i;
    for(i = 0; i < 20; i++){
        MapData_t *entity = &map[i];
        if(entity->entitytype){
            if(entity->mobile){
                if(!tick) proc_MoveEntity(&entity->position, entity->speed>>1);
                else proc_MoveEntity(&entity->position, entity->speed % 2);
                if(!entity->entitystats.weapon.range--) memset(entity, 0, sizeof(MapData_t));
            }
        }
    }
}
