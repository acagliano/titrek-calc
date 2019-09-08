
//The client-side space rendering for TI-trek-CE

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <tice.h>

#include <graphx.h>
#include <fileioc.h>

#include "gfx_engine.h"


//Sort objects by Z axis. Assumes they've already been transformed.
void SortByDepth(mapentity_t *objects, unsigned int len){
	
	
}

//Translate len objects by vector
void tfm_translate(mapentity_t *objects, unsigned int len, vec3_t vector){
	while (len--){
		
		
	}
}

//Rotate len objects by vector
void tfm_rotate(mapentity_t *objects, unsigned int len, rad2_t vector);

//Scale len objects by vector
void tfm_scale(mapentity_t *objects, unsigned int len, vec3_t vector);

//Server-side? Dummy data output for now, soon.
void GatherObjects(mapentity_t *objects, vec3_t start, uint8_t rotmin, uint8_t rotmax);



