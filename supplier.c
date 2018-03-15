#include <semaphore.h>
#include <unistd.h>

#include "environment.h"

void* supplier_run(void* arg) {
	int supplier_id = *(int*)arg;
	
	while (1) {
		sem_wait(&recycled_materials);
		sem_post(&ui_update);
		sleep(rand() % 3 + 2);
		sem_post(&stored_goods);
		sem_post(&ui_update);
	}
}
