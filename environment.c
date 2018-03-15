#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "environment.h"
#include "supplier.h"
#include "consumer.h"

void start_environment() {
	pthread_t suppliers[5];
	pthread_t consumers[5];
	
	sem_init(&stored_goods, 0, 0u);
	sem_init(&recycled_materials, 0, 5u);
	
	for (int i = 0; i < 5; ++i) {
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
	
	for (int i = 0; i < 5; ++i) {
		int* consumer_id = malloc(sizeof(int));
		*consumer_id = i + 20;
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
}
