#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ships.h"
#include "../gamestate.h"

bool ship_load_data(const uint8_t* data, size_t len){
	
	// (uint8_t)*data is the maximum module count
	// (uint8_t*)data+1 is the ptr to the start of module data
	// len - 1 is the length of the module data
	// (len - 1) / sizeof(shipmodule_t) is the number of modules
	
	uint8_t module_count = (len - 1) / sizeof(shipmodule_t);
	uint8_t *modules = malloc(len-1);
	if(!modules) return false;
	
	gamestate.ship.ship_module_max = *data++;
	gamestate.ship.ship_module_count = module_count;
	gamestate.ship.modules = (shipmodule_t*)modules;
	
	memcpy(modules, data, len-1);
	
	return true;
}
