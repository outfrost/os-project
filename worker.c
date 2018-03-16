#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "worker.h"
#include "environment.h"

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
		worker_update_state(worker, IDLE);
		sem_wait(input_pile);
		
		worker_update_state(worker, TAKING_ITEM);
		nanosleep(&interaction_time, NULL);
		
		worker_update_state(worker, PROCESSING);
		randomize_proc_time(&(*worker).processing_time);
		nanosleep(&(*worker).processing_time, NULL);
		
		worker_update_state(worker, STORING_ITEM);
		nanosleep(&interaction_time, NULL);
		sem_post(output_pile);
	}
	
	int* result = malloc(sizeof(int));
	*result = 0;
	return (void*)result;
}

void worker_update_state(Worker* worker, WorkerState state) {
	if ((*worker).type == SUPPLIER) {
		supplier_state[(*worker).id] = state;
	}
	else if ((*worker).type == CONSUMER) {
		consumer_state[(*worker).id] = state;
	}
	sem_post(&ui_update);
}

void randomize_proc_time(struct timespec* tspec) {
	(*tspec).tv_sec = random() % 3 + 2;
	(*tspec).tv_nsec = random() % 1000000000;
}
