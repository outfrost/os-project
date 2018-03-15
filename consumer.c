#include <semaphore.h>
#include <unistd.h>

#include "environment.h"

void* consumer_run(void* arg) {
	int consumer_id = *(int*)arg;
	
	while (1) {
		sem_wait(&stored_goods);
		sem_post(&ui_update);
		sleep(rand() % 3 + 2);
		sem_post(&recycled_materials);
		sem_post(&ui_update);
	}
}
