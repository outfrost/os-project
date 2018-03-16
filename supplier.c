#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "typedefs.h"
#include "environment.h"

void supplier_update_state(int id, WorkerState state) {
	supplier_state[id] = state;
	sem_post(&ui_update);
}

static void randomize_proc_time(struct timespec* tspec) {
	(*tspec).tv_sec = random() % 3 + 2;
	(*tspec).tv_nsec = random() % 1000000000;
}

void* supplier_run(void* arg) {
	int id = *(int*)arg;
	free(arg);
	arg = NULL;
	
	struct timespec processing_time;
	struct timespec interaction_time = { 1, 0 };
	
	while (1) {
		supplier_update_state(id, IDLE);
		sem_wait(&recycled_materials);
		
		supplier_update_state(id, TAKING_ITEM);
		nanosleep(&interaction_time, NULL);
		
		supplier_update_state(id, PROCESSING);
		randomize_proc_time(&processing_time);
		nanosleep(&processing_time, NULL);
		
		supplier_update_state(id, STORING_ITEM);
		nanosleep(&interaction_time, NULL);
		sem_post(&stored_goods);
	}
	
	int* result = malloc(sizeof(int));
	*result = 0;
	return (void*)result;
}
