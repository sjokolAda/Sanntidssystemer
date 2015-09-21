#include "io.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>


#define CHANNEL_A 1
#define CHANNEL_B 2
#define CHANNEL_C 3

void timespec_add_us(struct timespec *t, long us);

int set_cpu(int cpu_number) 
{ 
// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu); 
	CPU_SET(cpu_number, &cpu); 
// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu); 
} 




void* TestA()
{
	set_cpu(1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1){
		timespec_add_us(&next,100);
		io_write(CHANNEL_A, io_read(CHANNEL_A));		
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return NULL;
}

void* TestB()
{
	set_cpu(1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1){
		timespec_add_us(&next,100);
		io_write(CHANNEL_B, io_read(CHANNEL_B));		
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return NULL;
}

void* TestC()
{
	set_cpu(1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1){
		timespec_add_us(&next,100);
		io_write(CHANNEL_C, io_read(CHANNEL_C));		
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
	return NULL;
}

void* tarderthread(){
	set_cpu(1);
	int i = 1;
	while(1){
		i++;
		i--;
	}
}

void timespec_add_us(struct timespec *t, long us) 
{ 
	// add microseconds to timespecs nanosecond counter 
	t->tv_nsec += us*1000; 
	// if wrapping nanosecond counter, increment second counter 
	if (t->tv_nsec > 1000000000) 
	{ 
		t->tv_nsec = t->tv_nsec - 1000000000; 
		t->tv_sec += 1; 
	} 
} 

int main(){
	int res = io_init();
	if (res !=1){
		printf("IO init failed\n");
		return -1;
	}
	printf("Hello from main! IO success\n");
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	
	
	pthread_t dthread1;
	pthread_t dthread2;
	pthread_t dthread3;
	pthread_t dthread4;
	pthread_t dthread5;
	pthread_t dthread6;
	pthread_t dthread7;
	pthread_t dthread8;
	pthread_t dthread9;
	pthread_t dthread10;

	
	pthread_create(&thread1, NULL, TestA, NULL);	
	pthread_create(&thread2, NULL, TestB, NULL);
	pthread_create(&thread3, NULL, TestC, NULL);
	
	pthread_create(&dthread1, NULL, tarderthread, NULL);
	pthread_create(&dthread2, NULL, tarderthread, NULL);
	pthread_create(&dthread3, NULL, tarderthread, NULL);
	pthread_create(&dthread4, NULL, tarderthread, NULL);
	pthread_create(&dthread5, NULL, tarderthread, NULL);
	pthread_create(&dthread6, NULL, tarderthread, NULL);
	pthread_create(&dthread7, NULL, tarderthread, NULL);
	pthread_create(&dthread8, NULL, tarderthread, NULL);
	pthread_create(&dthread9, NULL, tarderthread, NULL);
	pthread_create(&dthread10, NULL, tarderthread, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	pthread_join(dthread1, NULL);
	pthread_join(dthread2, NULL);
	pthread_join(dthread3, NULL);
	pthread_join(dthread4, NULL);
	pthread_join(dthread5, NULL);
	pthread_join(dthread6, NULL);
	pthread_join(dthread7, NULL);
	pthread_join(dthread8, NULL);
	pthread_join(dthread9, NULL);
	pthread_join(dthread10, NULL);

	printf("Hello from main! Threads are done\n");
	return 0;

}
