
#include <ev.h>

listener_t listener_queue[QUEUE_MAX];
uint8_t queue_start = 0;
uint8_t queue_stop = 0;


bool enqueue(void (*exec)(), bool requeue){
	if(exec == NULL) return false;
	listener_t *lst = &listener_queue[queue_stop++];
	lst->exec = exec;
	lst->requeue = requeue;
}
