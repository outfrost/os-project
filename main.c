#include <pthread.h>
#include <semaphore.h>

#include "worker.h"
#include "environment.h"

int main(int argc, char** argv) {
	pthread_t ui_thread;
	
	start_environment(&ui_thread);
	
	pthread_join(ui_thread, NULL);
	terminate_environment();
	
	return 0;
}
