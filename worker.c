#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "environment.h"
#include "worker.h"

struct timespec interaction_time = { 0, 300000000L };

void* worker_run(void* arg) {
	Worker* worker = (Worker*)arg;
	
	sem_t* input_pile = NULL;
	sem_t* output_pile = NULL;
	if ((*worker).type == SUPPLIER) {
		input_pile = &recycled_materials;
		output_pile = &stored_goods;
	}
	else if ((*worker).type == CONSUMER) {
		input_pile = &stored_goods;
		output_pile = &recycled_materials;
	}
	
	while (1) {
		worker_update_state(worker, WORKER_IDLE);
		sem_wait(input_pile);
		
		worker_update_state(worker, WORKER_TAKING_ITEM);
		nanosleep(&interaction_time, NULL);
		
		worker_update_state(worker, WORKER_PROCESSING);
		randomize_proc_time(&(*worker).processing_time);
		nanosleep(&(*worker).processing_time, NULL);
		
		worker_update_state(worker, WORKER_STORING_ITEM);
		nanosleep(&interaction_time, NULL);
		sem_post(output_pile);
	}
	
	int* result = malloc(sizeof(int));
	*result = 0;
	return (void*)result;
}

void worker_update_state(Worker* worker, WorkerState state) {
	(*worker).state = state;
	sem_post(&ui_update);
}

void randomize_proc_time(struct timespec* tspec) {
	(*tspec).tv_sec = random() % 3 + 2;
	(*tspec).tv_nsec = random() % 1000000000;
}

char* worker_type_name(WorkerType type) {
	if (type == SUPPLIER) {
		return "Supplier";
	}
	else if (type == CONSUMER) {
		return "Consumer";
	}
	else {
		return "Unrecognized Worker";
	}
}
