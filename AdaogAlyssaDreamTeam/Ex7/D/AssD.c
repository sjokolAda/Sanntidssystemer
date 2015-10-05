#include "io.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/timer.h>
#include <rtdk.h>
#include <native/mutex.h>

#define TASK_PRIO_HIGHEST 99 /* Highest RT priority */
#define TASK_PRIO_HIGH 3 
#define TASK_PRIO_MEDIUM 2
#define TASK_PRIO_LOW 1

#define TASK_MODE T_JOINABLE /* Allow threads to join */
#define TASK_STKSZ 0 /* Stack size (use default one) */
#define cpuid 1 /*cpu number*/
#define U100 100000000


//task definition
RT_TASK LowTask;
RT_TASK HighTask;
RT_TASK Syncer;

RT_SEM semafor;
RT_MUTEX A;
RT_MUTEX B;


int set_cpu(int cpu_number) 
{ 
// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu); 
	CPU_SET(cpu_number, &cpu); 
// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu); 
} 


void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

void print_pri(RT_TASK *task, char *s)
{ 
	
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}

//Task_body

void lowTask (void* cookie)
{
	rt_printf("starting L\n");
	rt_sem_p(&semafor,TM_INFINITE);
	rt_printf("Im L passing barrier\n");

	rt_mutex_acquire(&A,TM_INFINITE);
		

	rt_printf("Im L using resource A\n");
	rt_timer_spin(300000);
	rt_mutex_acquire(&B,TM_INFINITE);
	
	rt_printf("Im L using resource B\n");
	rt_timer_spin(300000);

	rt_mutex_release(&B);
	rt_mutex_release(&A);
	rt_timer_spin(100000);
	rt_printf("L done\n");

}



void highTask(void* cookie)
{
	rt_printf("starting H\n");
	rt_sem_p(&semafor,TM_INFINITE);
	rt_printf("Im H passing barrier\n");

	rt_task_sleep(100000);	//sleeps task		

	rt_mutex_acquire(&B,TM_INFINITE);;
	rt_printf("Im H using resource B\n");
	rt_timer_spin(100000);

	rt_mutex_acquire(&A,TM_INFINITE);;
	rt_printf("Im H using resource A\n");
	rt_timer_spin(200000);
	
	
	rt_mutex_release(&A);
	rt_mutex_release(&B);
	rt_timer_spin(100000);
	rt_printf("H done\n");
	
}




int main(){
	
	int err;
	mlockall(MCL_CURRENT|MCL_FUTURE); //lock the current and future memory allocations to the main memory

	rt_print_auto_init(1);

	err = rt_sem_create(&semafor, "sem", 0,S_PRIO);  
	if (err){
		rt_printf("bad semaphore!%d",err);
	}
	
	//Create and init tasks
	
	
	err = rt_mutex_create(&A,"MyMutexA"); 
	if (err){
		rt_printf("bad resource!%d\n",err);
	}

	err = rt_mutex_create(&B,"MyMutexB"); 
	if (err){
		rt_printf("bad resource!%d\n",err);
	}
	
	

	err = rt_task_create(&LowTask,"LTASK",TASK_STKSZ,TASK_PRIO_LOW,TASK_MODE|T_CPU(cpuid));
	if (!err){
		
		rt_task_start(&LowTask,&lowTask,NULL);
	}

	err = rt_task_create(&HighTask,"HighTask",TASK_STKSZ,TASK_PRIO_HIGH,TASK_MODE|T_CPU(cpuid));
	if (!err){
		
		rt_task_start(&HighTask,&highTask,NULL);
	}

	rt_task_shadow(NULL, "main", TASK_PRIO_HIGHEST, T_CPU(cpuid));
	
	rt_printf("starting\n");
	rt_task_sleep(500000);
	err =  rt_sem_broadcast(&semafor); //broadcast semaphore
	rt_printf("broadcasting semaphore\n");
	if(err){
		rt_printf("sem broadcast fail\n");
	}
	rt_task_sleep(100000);	//sleeps task
	
	
	

	//join rt_threads	
	rt_task_join(&LowTask);
	rt_task_join(&HighTask);
	rt_task_join(&Syncer);
	


	//delete tasks and sempahore
	rt_task_delete(&LowTask);
	rt_task_delete(&HighTask);	
	rt_task_delete(&Syncer);	
	rt_sem_delete(&semafor);
	rt_mutex_delete(&A);
	rt_mutex_delete(&B);

	rt_printf("Hello from main! Threads are done and deleted\n");
	return 0;
}

