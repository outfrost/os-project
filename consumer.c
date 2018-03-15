#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#include "environment.h"

void* consumer_run(void* arg) {
	int consumer_id = *(int*)arg;
	
	while (1) {
		printf("Consumer %d looking for goods...\n", consumer_id);
		sem_wait(&stored_goods);
		printf("Consumer %d consuming\n", consumer_id);
		sleep(3);
		printf("Consumer %d finished consuming\n", consumer_id);
		sem_post(&recycled_materials);
	}
}
