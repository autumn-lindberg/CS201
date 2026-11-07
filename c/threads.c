#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// onThread function needs to be able to take in any argument and return any type
// you should know what the type needs to be, so you can safely cast it
void * onThread(void * arg) {
	(void) arg;
	// CRITICAL SECTION: one thread can run this code at a time (between lock and unlock)
	//
	// lock the the thread
	pthread_mutex_lock(&mutex);

	int temp = counter;

	for (int i = 0; i < 10000; i++) {
		temp++;
	}
	// set the "global" counter variable to the last value
	//
	// first thread: 1 - 10000
	// second thread: 10001 - 20000
	// third thread: 20001 - 30000
	// ...
	counter = temp;

	// unlock the thread
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main() {

	// 
	pthread_mutex_init(&mutex, NULL);
	pthread_t threads[100];

	for (int i = 0; i < 100; i++) {
		pthread_create(&threads[i], NULL, onThread, NULL);
	}

	// PTHREAD JOIN MEANS WAIT HERE TIL THREAD FINISHES
	for (int i = 0; i < 100; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("counter: %d\n", counter);
}
