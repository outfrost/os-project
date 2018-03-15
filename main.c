#include <pthread.h>
#include <semaphore.h>

#include "environment.h"

int main(int argc, char** argv) {
	start_environment();
	
	pthread_exit(NULL);
	
	return 0;
}
