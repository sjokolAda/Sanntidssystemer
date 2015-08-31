#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


int global = 0;


int main(void)
{
	int incr = 10;
	
	pid_t pid;

	fork();
	//vfork();
	int local = 0;
	pid = getpid();
	
	int i;
	for (i=0;i<incr;i++)
	{
		global++;
		local++;
	}
	printf("This line if from pid %d, local value = %d, global value= %d \n", pid, local, global);	
	return 0;
}


	
