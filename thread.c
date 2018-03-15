#include <stdlib.h>
#include <stdio.h>

void* run(void* arg) {
	printf("1\n");
	printf("2\n");
	printf("3\n");
	printf("4\n");
	printf("clap\n");
	printf("6\n");
	printf("7\n");
	printf("8\n");
	printf("9\n");
	printf("clap\n");
	printf("11\n");
	printf("12\n");
	printf("13\n");
	printf("14\n");
	printf("clap\n");
	printf("16\n");
	printf("17\n");
	printf("18\n");
	printf("19\n");
	printf("clap\n");
	int* result = malloc(sizeof(int));
	*result = 0;
	return (void*)result;
}
