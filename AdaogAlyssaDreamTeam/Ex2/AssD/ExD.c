#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>


int running = 1;
int var1 = 0;
int var2 = 0;

pthread_mutex_t lock;

void* Thread1()
{
	while (running)
	{
	pthread_mutex_lock(&lock);
	    var1 = var1 + 1;
	    var2 = var1;
	pthread_mutex_unlock(&lock);
	}
  	
	return NULL;	
}

void* Thread2()
{	

	int i;
	for (i=1;i<21;i++)
	{
		pthread_mutex_lock(&lock);
	    	printf("Number 1 is %d , number2 is %d\n", var1, var2);
		pthread_mutex_unlock(&lock);
	    	usleep(100000);

	}
	running = 0;
	return NULL;
}

int main(){

	if (pthread_mutex_init(&lock, NULL) != 0)
    	{
        	perror("mutex initilization");
		exit(0);
 
   	}

	pthread_t thread1;
	pthread_t thread2;

    	pthread_create(&thread1, NULL, Thread1, NULL);
	pthread_create(&thread2, NULL, Thread2, NULL);
	
	
	pthread_join(thread1, NULL);
	pthread_join(thread2,NULL);
    	printf("Hello from main! Threads are done\n");
	
return 0;
}
