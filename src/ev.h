#ifndef EV_H
#define EV_H

#include <stdint.h>
#include <stdbool.h>

enum _proc_ids {
	PROC_SKIP,
	PROC_NTWK,
	PROC_KEYDETECT,
	PROC_RENDER
};

#define QUEUE_MAX 256
typedef struct _listener_t {
	uint8_t proc_id;
	void (*exec)();
	bool requeue;
} listener_t;

extern listener_t listener_queue[QUEUE_MAX];
extern uint8_t queue_start;
extern uint8_t queue_stop;

bool enqueue(void (*exec)(), uint8_t proc_id, bool requeue);
void dequeue(uint8_t proc_id, uint8_t max_count);

#endif
