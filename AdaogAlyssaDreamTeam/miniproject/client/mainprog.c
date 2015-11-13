#include <arpa/inet.h>
#include <pthread.h>
#include "miniproject.h"
#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"



#define UDP_PORT = 9999;
#define SERVER_IP = 129.241.187.146;
#define MSG_LENGTH = 255;



//Mutexes and semaphores
pthread_mutex_t TINFOlock;
pthread_mutex_t singlesend;
sem_t u_updt;
sem_t y_updt;

struct udp_conn client;
struct TINFO
{
	double y;
	double u;
	struct udp_conn conn;
};


//functions
void UDP_listener(*t_info);
void UDP_sender(*t_info);
void PIctrl(*t_info);


int main()
{
	// init UDP connection struct
	struct TINFO *t_info = malloc(sizeof(*t_info));

	t_info->y = 0;
	t_info->u = 0;
	t_info->conn=client;


	udp_init_client(&client, UDP_PORT, SERVER_IP); //or own ip?
	
	//init variables/semaforer/mutexer
	int erru = sem_init(&u_updt,0,0);
	int erry = sem_init(&y_udp,0,0);
	if ((erry != 0)||(erry!=0))
	{
		perror("Sem init failed");
	}
	int errv= pthread_mutex_init(&TINFOlock,NULL);
	int errs = pthread_mutex_init(&singlesend, NULL);	
	if ((errv!=0)||(errs!=0)
	{
		perror("Mutex init failed");
	}


    //start threads
	pthread_t udprecv, udpsend, pictrl;
	int res = pthread_create_(&udprecv, NULL, UDP_listener);
	if (res!=0)
	{
		perror("Thread Creation failed");
	}

	res = pthread_create_(&udpsend, NULL, UDP_sender);
	if (res!=0)
	{
		perror("Thread Creation failed");
	}

	res = pthread_create_(&pictrl, NULL, PI_controller);
	if (res!=0)
	{
		perror("Thread Creation failed");
	}	
	
	
	//join thread
	res = pthread_join(udprecv, NULL);  
    if (res != 0) {  
        perror("Joining of thread failed stooped");  
        exit(EXIT_FAILURE);  
    } 

	res = pthread_join(udpsend, NULL);  
    if (res != 0) {  
        perror("Joining of thread failed stooped");  
        exit(EXIT_FAILURE);  
    }

	res = pthread_join(pictrl, NULL);  
    if (res != 0) {  
        perror("Joining of thread failed stooped");  
        exit(EXIT_FAILURE);  
    }
	
	//free allocate mem
	free(t_info); 

	// close connection
	udp_close(t_info->con);	
	
    
	//Destroy all semaphores and mutexes
	
	sem_destroy(&u_udpt);
	sem_destroy(&y_udpt);
	ptread_mutex_destroy(&varlock);

	
	
	return 0;      
}


 void *UDP_listener(void *t_info)
{
	struct TINFO *tinfo = t_info;
	char buf[MSG_LENGHT];
	double value;

	while(1){
		int err = udp_receive(&(tinfo->conn), buf, MSG_LENGHT);
		if(err!=0){
			perror("Udp_receive failed");
		}
		pthread_mutex_lock(&TINFOlock);
		tinfo->y = atof(buf);				
		
		pthread_mutex_unlock(&TINFOlock);
		sem_post(&y_updt);							
	}	
}
		

void *UPD_sender(void *t_info)
{	
	char buf[MSG_LENGTH];
	sem_wait(&u_updt);
	pthread_mutex_lock(&TINFOlock);	
	itoa(u,buf,10);
	pthread_mytex_unlock(&TINFOlock);
}

