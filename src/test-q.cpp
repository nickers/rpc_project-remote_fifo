#include "data_queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

data_queue q;

void* feeder(void* arg) {
	char* a = (char*)arg;
	for (int i=0; i<strlen(a); i++) {
		q.write(1, &a[i]);
		sleep(1);
	}
	return arg;
}

pthread_t feeders[5];

int main(int argc, char* argv[])
{
	char put[] = "test";
	q.write(strlen(put), put);
	printf("put: [%s]\n", put);

	for (int i=0; i<5; i++)
		pthread_create(&feeders[i], NULL, feeder, put);

	printf("workers started\n");fflush(0);

	for (int i=0; i<24; i++) {
		q.read(1, put);
		printf("%d) [%c]\n", i, put[0]);
		fflush(0);
	}

	return 0;
}
