
#include <ev.h>

listener_t listener_queue[QUEUE_MAX];
uint8_t queue_start = 0;
uint8_t queue_stop = 0;


bool enqueue(int (exec*)(), listener_data_t* data, bool requeue){
	if(exec == NULL) return false;
	if(data == NULL) return false;
	listener_queue[queue_stop++] = {exec, data, requeue};
}
