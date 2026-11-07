#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define STDOUT 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
volatile sig_atomic_t running = 1;
int led = 0;

void * servoFunc(void * arg) {
	(void) arg;

	FILE * fptr; 
	time_t currentTime;
	char * datetime;

	// "calibrate" servo
	printf("Calibrating servo...\n");

	// wait until all calibrations are done
	pthread_barrier_wait(&barrier);
	
	// wait a second to show calibration is done
	sleep(3);

	while (running) {

		// lock the thread
		pthread_mutex_lock(&mutex);
		
		// open file in append mode
		fptr = fopen("logfile", "a");	
		if (fptr == NULL) {
			printf("Unable to open file\n");
		}
		else {
			printf("Servo changed direction. Writing to logfile...\n");
			
			// write to file
			currentTime = time(NULL);
			datetime = ctime(&currentTime);
			datetime[strlen(datetime) - 1] = '\0';
			fprintf(fptr, "%s: servo changed direction\n", datetime);

			// close file
			fclose(fptr);
		}
		pthread_mutex_unlock(&mutex);

		// wait another few seconds to do it again
		sleep(rand() % 4 + 1);
	}

	printf("Shutting down servo...\n");
	return NULL;
}

void * sensorFunc(void * arg) {
	(void) arg;

	FILE * fptr; 
	time_t currentTime;
	char * datetime;

	// "calibrate" sensor
	printf("Calibrating distane sensor...\n");

	// wait until all calibrations are done
	pthread_barrier_wait(&barrier);

	// wait a second to show calibration is done
	sleep(3);

	while (running) {

		// lock the thread
		pthread_mutex_lock(&mutex);
		
		// open file in append mode
		fptr = fopen("logfile", "a");	
		if (fptr == NULL) {
			printf("Unable to open file\n");
		}
		else {
			printf("Sensor detected a wall. Writing to logfile...\n");
			
			// write to file
			currentTime = time(NULL);
			datetime = ctime(&currentTime);
			datetime[strlen(datetime) - 1] = '\0';
			fprintf(fptr, "%s: sensor detected a wall\n", datetime);

			// close file
			fclose(fptr);
		}
		pthread_mutex_unlock(&mutex);

		// wait another few seconds to do it again
		sleep(rand() % 4 + 1);
	}

	printf("Shutting down sensor...\n");
	return NULL;
}

void * ledFunc(void * arg) {
	(void) arg;

	FILE * fptr; 
	time_t currentTime;
	char * datetime;

	// "calibrate" LED
	printf("Calibrating LED...\n");

	// wait until all calibrations are done
	pthread_barrier_wait(&barrier);
	
	// wait a second to show calibration is done
	sleep(3);

	while (running) {

		// lock the thread
		pthread_mutex_lock(&mutex);
		
		// open file in append mode
		fptr = fopen("logfile", "a");	
		if (fptr == NULL) {
			printf("Unable to open file\n");
		}
		else {
			printf("LED value changed. Writing to logfile...\n");
			
			// write to file
			currentTime = time(NULL);
			datetime = ctime(&currentTime);
			datetime[strlen(datetime) - 1] = '\0';
			fprintf(fptr, "%s: LED changed to %d\n", datetime, led);

			// close file
			fclose(fptr);
		}
		pthread_mutex_unlock(&mutex);

		// wait another few seconds to do it again
		sleep(rand() % 4 + 1);
	}
	
	printf("Shutting down LED...\n");
	return NULL;
}

void turnOff() {
	
	// async safe printing
	char buffer[80];
	int length = sprintf(buffer, "\n\nInitiating power down...\n");
	write(STDOUT, buffer, length);

	// turn off the loop
	running = 0;

	return;
}

void * powerFunc (void * arg) {
	(void) arg;
	int sig;

	// wait for ctrl + c to be pressed
	signal(SIGINT, turnOff);

	return NULL;
}

int main() {
	// initialize mutual exclusion
	pthread_mutex_init(&mutex, NULL);
	// initialize barrier for 3 threads to wait at
	pthread_barrier_init(&barrier, NULL, 3);

	// define threads
	pthread_t servo;
	pthread_t distance_sensor;
	pthread_t LED;
	pthread_t power_button;

	// initialize threads
	pthread_create(&servo, NULL, servoFunc, NULL);
	pthread_create(&distance_sensor, NULL, sensorFunc, NULL);
	pthread_create(&LED, NULL, ledFunc, NULL);
	pthread_create(&power_button, NULL, powerFunc, NULL);

	// wait for threads to finish
	pthread_join(servo, NULL);
	pthread_join(distance_sensor, NULL);
	pthread_join(LED, NULL);
	pthread_join(power_button, NULL);
	
	// cleanup
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

}
