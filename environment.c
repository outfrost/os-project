#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "worker.h"
#include "ui.h"
#include "environment.h"

void start_environment(pthread_t* ui_thread) {
	srandom(time(NULL));
	init_semaphores();
	spawn_workers();
	start_ui(ui_thread);
	sem_post(&ui_update);
}

void terminate_environment() {
	terminate_workers();
	destroy_semaphores();
}

void init_semaphores() {
	sem_init(&stored_goods, 0, 0u);
	sem_init(&recycled_materials, 0, INITIAL_ITEMS);
	sem_init(&ui_update, 0, 0u);
}

void destroy_semaphores() {
	sem_destroy(&stored_goods);
	sem_destroy(&recycled_materials);
	sem_destroy(&ui_update);
}

void spawn_workers() {
	for (int i = 0; i < WORKERS_COUNT; ++i) {
		workers[i].id = i;
		if (i < SUPPLIERS_COUNT) {
			workers[i].type = SUPPLIER;
		}
		else {
			workers[i].type = CONSUMER;
		}
		
		int create_status = pthread_create(&workers[i].thread, NULL, worker_run, (void*)&workers[i]);
		if (create_status != 0) {
			fprintf(stderr, "Cannot spawn %s %d: ", worker_type_name(workers[i].type), i);
			if (create_status == EAGAIN) {
				fprintf(stderr, "Cannot allocate resources for thread\n");
			}
			else if (create_status == EINVAL || create_status == EPERM) {
				fprintf(stderr, "Invalid or forbidden thread creation attributes\n");
			}
			else {
				fprintf(stderr, "Unknown error (%d)\n", create_status);
			}
		}
		else {
			printf("%s %d spawned\n", worker_type_name(workers[i].type), i);
		}
	}
}

void terminate_workers() {
	for (int i = 0; i < WORKERS_COUNT; ++i) {
		pthread_cancel(workers[i].thread);
	}
	
	for (int i = 0; i < WORKERS_COUNT; ++i) {
		void* retval = NULL;
		pthread_join(workers[i].thread, &retval);
		
		printf("%s %d terminated (", worker_type_name(workers[i].type), i);
		if (retval == PTHREAD_CANCELED) {
			printf("Thread canceled");
		}
		else {
			printf("Exit status: %d", *(int*)retval);
		}
		printf(")\n");
	}
}

void start_ui(pthread_t* ui_thread) {
	int create_status = pthread_create(ui_thread, NULL, ui_run, NULL);
	if (create_status != 0) {
		fprintf(stderr, "Cannot start UI: ");
		if (create_status == EAGAIN) {
			fprintf(stderr, "Cannot allocate resources for thread\n");
		}
		else if (create_status == EINVAL || create_status == EPERM) {
			fprintf(stderr, "Invalid or forbidden thread creation attributes\n");
		}
		else {
			fprintf(stderr, "Unknown error (%d)\n", create_status);
		}
	}
}
