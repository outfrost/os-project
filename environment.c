#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "typedefs.h"
#include "environment.h"
#include "supplier.h"
#include "consumer.h"
#include "profanities.h"

void start_environment(pthread_t* ui_thread) {
	sem_init(&stored_goods, 0, 0u);
	sem_init(&recycled_materials, 0, INITIAL_ITEMS);
	
	srandom(time(NULL));
	
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		int* supplier_id = malloc(sizeof(int));
		*supplier_id = i;
		int create_status = pthread_create(&suppliers[i], NULL, supplier_run, (void*)supplier_id);
		if (create_status != 0) {
			if (create_status == EAGAIN) {
				fprintf(stderr, "Cannot spawn supplier %d: Cannot allocate resources for thread\n", i);
			}
			else if (create_status == EINVAL || create_status == EPERM) {
				fprintf(stderr, "Cannot spawn supplier %d: Invalid or forbidden thread creation attributes\n", i);
			}
		}
	}
	
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		int* consumer_id = malloc(sizeof(int));
		*consumer_id = i;
		int create_status = pthread_create(&consumers[i], NULL, consumer_run, (void*)consumer_id);
		if (create_status != 0) {
			if (create_status == EAGAIN) {
				fprintf(stderr, "Cannot spawn consumer %d: Cannot allocate resources for thread\n", i);
			}
			else if (create_status == EINVAL || create_status == EPERM) {
				fprintf(stderr, "Cannot spawn consumer %d: Invalid or forbidden thread creation attributes\n", i);
			}
		}
	}
	
	int create_status = pthread_create(ui_thread, NULL, profanities_run, NULL);
	if (create_status != 0) {
		if (create_status == EAGAIN) {
			fprintf(stderr, "Cannot start profanities: Cannot allocate resources for thread\n");
		}
		else if (create_status == EINVAL || create_status == EPERM) {
			fprintf(stderr, "Cannot start profanities: Invalid or forbidden thread creation attributes\n");
		}
	}
	
	sem_post(&ui_update);
}
