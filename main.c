#include <pthread.h>
#include <semaphore.h>

#include "environment.h"

int main(int argc, char** argv) {
	pthread_t ui_thread;
	
	start_environment(&ui_thread);
	/*
	pthread_exit(NULL);
	*/
	pthread_join(ui_thread, NULL);
	
	return 0;
}
