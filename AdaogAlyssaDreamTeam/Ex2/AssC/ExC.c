#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>

sem_t semawhore;




void* Thread()
{
	

	unsigned int tid;
	tid = (unsigned int)pthread_self();
	int i;
	sem_wait(&semawhore);
	for (i=0;i<5;i++)
	
	{
		
		printf("Thread number +i  %u\n", tid+i);
		usleep(1000000);
		
	}
	sem_post(&semawhore);

    	return NULL;
}

int main()
{

	
	sem_init(&semawhore, 0, 3);
	int value;
	sem_getvalue(&semawhore, &value); 
	printf("The value of the semaphors is %d\n", value);


	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;

    	pthread_create(&thread1, NULL, Thread, NULL);
	pthread_create(&thread2, NULL, Thread, NULL);
	pthread_create(&thread3, NULL, Thread, NULL);
	pthread_create(&thread4, NULL, Thread, NULL);
	pthread_create(&thread5, NULL, Thread, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);

    	printf("Hello from main! Threads are done\n");
	sem_getvalue(&semawhore, &value); 
	printf("The value of the semaphors is %d\n", value);
	return 0;
}
