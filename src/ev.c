
#include <ev.h>

listener_t listener_queue[QUEUE_MAX];
uint8_t queue_start = 0;
uint8_t queue_stop = 0;


bool enqueue(void (*exec)(), uint8_t proc_id, bool requeue){
// pushes a function into the event queue
	if(exec == NULL) return false;
	listener_t *lst = &listener_queue[queue_stop++];
	lst->proc_id = proc_id;
	lst->exec = exec;
	lst->requeue = requeue;
	return true;
}

void dequeue(uint8_t proc_id, uint8_t max_count){
// removes any functions of proc_id from the event queue (up to max_count functions)
	for(int i=queue_start; i<queue_stop; i++){
		listener_t *lst = &listener_queue[queue_stop++];
		if(proc_id == lst->proc_id){
			lst->proc_id = PROC_SKIP;
			if(!max_count--) break;
		}
	}
}
