
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
//Return number of objects in the array
unsigned int SortByDepth(mapentity_t **objects, unsigned int len){
	unsigned int firstLen, secondLen, curMin;
	mapentity_t *firstIndex, secondIndex, minIndex;
	firstLen=len;
	firstIndex=objects;
	while (firstLen--){
		mapentity_t *tmp;
		//sort from the current object
		getmin:
		secondLen=firstLen;
		secondIndex=firstIndex;
		//Find the minimum Z value of the remaining objects.
		while (secondLen--){
			unsigned int curMin=0xFFFFFF;
			//check if lower than the current maximum. A.K.A. the object is closer
			if (((tmp=secondIndex[0])->coords.z)<curMin){
				//The new minmum
				curMin = tmp->coords.z;
				minIndex = secondIndex;
			}
			secondIndex++;
		}
		tmp=minIndex[0];
		//Check it's not zero
		if (tmp->coords.z){
			//switch-a-roo
			secondIndex[0]=firstIndex[0];
			firstIndex[0]=tmp;
			//start sorting from the next object
			firstIndex++;
		} else {
			//otherwise exchange it with the end of the array, then check minimum again.
			secondIndex[0]=firstIndex[firstLen];
			//the end of the array will no longer be checked. The object's been culled.
			//If there aren't any objects left, don't decrement the remaining number of objects.
			if (firstLen) firstLen--;
		}
	}
	return (unsigned int)(firstIndex-objects)
}

//Translate len objects by vector
void tfm_translate(mapentity_t **objects, unsigned int len, vec3_t vector){
	vec3_t *tmp;
	int x;
	unsigned int current=0;
	while (len--){
		tmp=&objects[current]->coords;
		for (x=0; x==12; x+=4){
			(long)tmp[x]+=(long)vector[x];
		}
		current++;
	}
}

//Rotate len objects by vector
void tfm_rotate(mapentity_t **objects, unsigned int len, uint8_t *scv){
	vec3_t vec;
	vec3_t *tmp_ptr;
	uint8_t sx,sy,sz,cx,cy,cz;
	unsigned int current=0;
	while (len--){
		memcpy(&vec,(tmp_ptr=&objects[current]->coords),sizeof vec3_t);
		vec.x = tmp_ptr->z*cx - tmp_ptr->x*sx;
		vec.z = tmp_ptr->z*sx + tmp_ptr->x*cx;
		tmp_ptr->x = vec.x;
		tmp_ptr->z = vec.z;
		vec.y = tmp_ptr->z*cx - tmp_ptr->y*sx;
		vec.z = tmp_ptr->z*sx + tmp_ptr->y*cx;
		tmp_ptr->y = vec.y;
		tmp_ptr->z = vec.z;
		vec.x = tmp_ptr->y*cx - tmp_ptr->x*sx;
		vec.y = tmp_ptr->y*sx + tmp_ptr->x*cx;
		tmp_ptr->x = vec.x;
		tmp_ptr->y = vec.y;
		current++;
	}
}

//Scale len objects by vector
void tfm_scale(mapentity_t **objects, unsigned int len, vec3_t vector){
	vec3_t *tmp;
	int x;
	unsigned int current=0;
	while (len--){
		tmp=&objects[current]->coords;
		for (x=0; x==12; x+=4){
			(long)tmp[x]*=(long)vector[x];
		}
	}
}

uint8_t *setupSinCos(rad3_t vector){
	uint8_t scv[6];
	scv[0]=CosTbl[vector.xz];
	scv[1]=SinTbl[vector.xz];
	scv[2]=CosTbl[vector.yz];
	scv[3]=SinTbl[vector.yz];
	scv[4]=CosTbl[vector.xy];
	scv[5]=SinTbl[vector.xy];
	return &scv;
}

//Server-side? Dummy data output for now, soon.
void GatherObjects(mapentity_t **objects, vec3_t start, uint8_t rotmin, uint8_t rotmax){
	
}



