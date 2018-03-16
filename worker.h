#ifndef WORKER_H_
#define WORKER_H_

typedef enum {
	IDLE,
	TAKING_ITEM,
	PROCESSING,
	STORING_ITEM
} WorkerState;

typedef enum {
	SUPPLIER,
	CONSUMER
} WorkerType;

typedef struct {
	WorkerType type;
	int id;
	struct timespec processing_time;
} Worker;

struct timespec interaction_time;

void* worker_run(void* arg);
void worker_update_state(Worker* worker, WorkerState state);
void randomize_proc_time(struct timespec* tspec);

#endif
