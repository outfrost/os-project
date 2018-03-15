#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#include "environment.h"

void* supplier_run(void* arg) {
	int supplier_id = *(int*)arg;
	
	while (1) {
		printf("Supplier %d looking for recycled materials...\n", supplier_id);
		sem_wait(&recycled_materials);
		printf("Supplier %d processing\n", supplier_id);
		sleep(3);
		printf("Supplier %d finished processing\n", supplier_id);
		sem_post(&stored_goods);
	}
}
