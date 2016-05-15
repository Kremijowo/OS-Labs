

/*
 *	OS Lab #1.
 *	The dining philosophers problem solving using 
 *	mutex. OS Linux.
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 *
 *	compile this using g++ -pthread lab_1_philosophers.cpp -o philosophers
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#define PHILOSOPHERS_COUNT 5
#define MIN_THINKING_TIME 500000
#define MAX_THINKING_TIME 1000000
#define MIN_EATING_TIME 500000
#define MAX_EATING_TIME 1000000

pthread_mutex_t resourses[PHILOSOPHERS_COUNT];
pthread_t threads[PHILOSOPHERS_COUNT]; 

void* PhilosopherTask(void* id);
void Think(int id);
void Eat(int id);

int main(){
	srand(time(NULL));
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		if (pthread_mutex_init(&resourses[i], NULL) != 0){
			std::cout << "Mutex init failed!\n";
			return 1;
		}
	}

	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		if (pthread_create(&threads[i], NULL, PhilosopherTask, (void*)i) != 0){
			std::cout << "Threads creation failed!\n";
			return 1;
		}
	}

	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		pthread_mutex_destroy(&resourses[i]);
	}
}

void* PhilosopherTask(void* id){
	int philosopher_id = (int)id;
	int resourse1_id = (int)id;
	int resourse2_id = ((int)id + 1) % PHILOSOPHERS_COUNT;

	while(true) {
		Think(philosopher_id);
		pthread_mutex_lock(&resourses[resourse1_id]);
		pthread_mutex_lock(&resourses[resourse2_id]);
		Eat(philosopher_id);
		pthread_mutex_unlock(&resourses[resourse2_id]);
		pthread_mutex_unlock(&resourses[resourse1_id]);
	}
}

void Think(int id){
	int min = MIN_THINKING_TIME;
	int max = MAX_THINKING_TIME;

	usleep(rand() % (max - min) + min);
}

void Eat(int id){
	int min = MIN_EATING_TIME;
	int max = MAX_EATING_TIME;

	std::cout << "Philosopher #" << id << " start eating.\n";
	usleep(rand() % (max - min) + min);
	std::cout << "Philosopher #" << id << " end eating.\n";
}