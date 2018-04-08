#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "worker.h"
#include "environment.h"
#include "profanities.h"

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
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		Worker* worker = malloc(sizeof(Worker));
		(*worker).type = SUPPLIER;
		(*worker).id = i;
		int create_status = pthread_create(&suppliers[i], NULL, worker_run, (void*)worker);
		if (create_status != 0) {
			if (create_status == EAGAIN) {
				fprintf(stderr, "Cannot spawn supplier %d: Cannot allocate resources for thread\n", i);
			}
			else if (create_status == EINVAL || create_status == EPERM) {
				fprintf(stderr, "Cannot spawn supplier %d: Invalid or forbidden thread creation attributes\n", i);
			}
		}
		else {
			printf("Supplier %d spawned\n", i);
		}
	}
	
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		Worker* worker = malloc(sizeof(Worker));
		(*worker).type = CONSUMER;
		(*worker).id = i;
		int create_status = pthread_create(&consumers[i], NULL, worker_run, (void*)worker);
		if (create_status != 0) {
			if (create_status == EAGAIN) {
				fprintf(stderr, "Cannot spawn consumer %d: Cannot allocate resources for thread\n", i);
			}
			else if (create_status == EINVAL || create_status == EPERM) {
				fprintf(stderr, "Cannot spawn consumer %d: Invalid or forbidden thread creation attributes\n", i);
			}
		}
		else {
			printf("Consumer %d spawned\n", i);
		}
	}
}

void terminate_workers() {
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		pthread_cancel(suppliers[i]);
	}
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		pthread_cancel(consumers[i]);
	}
	
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		void* retval = NULL;
		pthread_join(suppliers[i], &retval);
		printf("Supplier %d terminated (", i);
		if (retval == PTHREAD_CANCELED) {
			printf("Thread canceled");
		}
		else {
			printf("Exit status: %d", *(int*)retval);
		}
		printf(")\n");
	}
	
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		void* retval = NULL;
		pthread_join(consumers[i], &retval);
		printf("Consumer %d terminated (", i);
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
		if (create_status == EAGAIN) {
			fprintf(stderr, "Cannot start profanities: Cannot allocate resources for thread\n");
		}
		else if (create_status == EINVAL || create_status == EPERM) {
			fprintf(stderr, "Cannot start profanities: Invalid or forbidden thread creation attributes\n");
		}
	}
}
