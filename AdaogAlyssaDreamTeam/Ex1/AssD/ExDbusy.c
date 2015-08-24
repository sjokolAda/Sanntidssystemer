#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

void print1();
void print2();
void busy_wait_delay(int seconds);

int main(){
	pthread_t thread1, thread2;
	
	pthread_create(&thread1, NULL, print1, NULL);
	pthread_create(&thread2, NULL, print2, NULL);
	
	
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 
	
	return 0;
	
}

void print1(){
	printf("Thread 1 - 1\n");
	busy_wait_delay(5);
	printf("Thread 1 - 2\n");
	return;
}

void print2(){
	printf("Thread 2 - 1\n");
	busy_wait_delay(5);
	printf("Thread 2 - 2\n");
	return;
}

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
		dummy = i;
		}
		times(&exec_time);
	}
}
