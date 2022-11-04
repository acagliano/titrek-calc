#ifndef EV_H
#define EV_H

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_MAX 256
typedef struct _listener_t {
	void (exec*)();
	bool requeue;
} listener_t;

extern listener_t listener_queue[QUEUE_MAX];
extern uint8_t queue_start;
extern uint8_t queue_stop;

bool enqueue(void (exec*)(), bool requeue);

#endif
