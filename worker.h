#ifndef WORKER_H_
#define WORKER_H_

typedef enum {
	WORKER_IDLE,
	WORKER_TAKING_ITEM,
	WORKER_PROCESSING,
	WORKER_STORING_ITEM
} WorkerState;

typedef enum {
	SUPPLIER,
	CONSUMER
} WorkerType;

typedef struct {
	int id;
	WorkerType type;
	WorkerState state;
	struct timespec processing_time;
	pthread_t thread;
} Worker;

struct timespec interaction_time;

void* worker_run(void* arg);
void worker_update_state(Worker* worker, WorkerState state);
void randomize_proc_time(struct timespec* tspec);
char* worker_type_name(WorkerType type);

#endif
