#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global = 0;


void* Incrementer(){
	

	printf("Hello from a thread 1!\n");
		
	int local = 0;
	int incr = 10;
	int i;
	for (i=0;i<incr;i++)
	{
		global++;
		local++;
	}
	printf("This is serious thread. local value = %d, global value= %d \n", local, global);
    	return NULL;
}

/*
void* Thread2(){

		printf("Hello from a thread 2!\n");
		sleep(5);
    	return NULL;
}
*/

int main(){
	pthread_t thread1;
	pthread_t thread2;

    	pthread_create(&thread1, NULL, Incrementer, NULL);
	pthread_create(&thread2, NULL, Incrementer, NULL);
	
	
	pthread_join(thread1, NULL);
	pthread_join(thread2,NULL);
    	printf("Hello from main! Threads are done\n");
	
return 0;
}
