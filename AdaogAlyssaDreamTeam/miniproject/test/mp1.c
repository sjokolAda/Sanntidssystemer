#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#include <semaphore.h>
#include "miniproject.h"
#include <pthread.h>


sem_t y_value_received;
sem_t new_u;
sem_t new_signal;

pthread_mutex_t send_lock;
pthread_mutex_t y_mut; 
pthread_mutex_t u_mut;  

void *udp_listner(void *arg){

	struct thread_args *args = arg;
	//struct udp_conn *udp, double *y
	while(1){
		char buffer[MESSAGE_LENGTH];
		int res = udp_receive(&(args->udp), buffer, MESSAGE_LENGTH);
		if (res == -1){
			return -1;
		}
		if(strcmp(buffer, "SIGNAL") == 0){
			sem_post(&new_signal);
		} else{
			pthread_mutex_lock(&y_mut);
			args->y = atof(buffer+8);
			pthread_mutex_unlock(&y_mut);
			sem_post(&y_value_received);
		}
	}
}

void *PI_controller(void *arg){
	double error = 0;
	double integral = 0;
	struct thread_args *args = arg;
	while(1){
		sem_wait(&y_value_received);
		pthread_mutex_lock(&y_mut);
		pthread_mutex_lock(&u_mut);

		error = (double)REFERENCE - (args->y);
		integral = integral + (error*(double)PERIOD);
		(args->u) = (double)Kp*error + (double)Ki*integral;

		pthread_mutex_unlock(&y_mut);
		pthread_mutex_unlock(&u_mut);
		sem_post(&new_u);
		
		
	}
}

void *simulate(void *arg){
	struct thread_args *args = arg;

	pthread_mutex_lock(&send_lock);
	udp_send(&(args->udp), "START", MESSAGE_LENGTH);
	pthread_mutex_unlock(&send_lock);

	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	while(1){	

		pthread_mutex_lock(&send_lock);
		udp_send(&(args->udp), "GET", MESSAGE_LENGTH);
		pthread_mutex_unlock(&send_lock);
		sem_wait(&new_u);

		char msg[MESSAGE_LENGTH];
		snprintf(msg, MESSAGE_LENGTH, "SET:%f", args->u); 

		pthread_mutex_lock(&send_lock);
		udp_send(&(args->udp), msg, MESSAGE_LENGTH);
		pthread_mutex_unlock(&send_lock);
			
		timespec_add_us(&time, PERIOD*1000000);
		clock_nanosleep(&time);


	}
	
}

void *stop_simulation(void *arg){
	struct thread_args *args = arg;
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	timespec_add_us(&time, 500000);
	clock_nanosleep(&time);

	pthread_mutex_lock(&send_lock);
	udp_send(&(args->udp), "STOP", MESSAGE_LENGTH);
	pthread_mutex_unlock(&send_lock);

	exit(0);

}

void *reply_to_signal(void *arg){
	struct thread_args *args = arg;
	while(1){
		sem_wait(&new_signal);
		udp_send(&(args->udp), "SIGNAL_ACK", MESSAGE_LENGTH);
	}

}

int udp_init_client(struct udp_conn *udp, int port, char *ip)
{
	struct hostent *host;

	if ((host = gethostbyname(ip)) == NULL) return -1;

	udp->client_len = sizeof(udp->client);
	// define servers
	memset((char *)&(udp->server), 0, sizeof(udp->server));
	udp->server.sin_family = AF_INET;
	udp->server.sin_port = htons(port);
	bcopy((char *)host->h_addr, (char *)&(udp->server).sin_addr.s_addr, host->h_length);

	// open socket
	if ((udp->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) return udp->sock;

	return 0;
}

int udp_send(struct udp_conn *udp, char *buf, int len)
{
	return sendto(udp->sock, buf, len, 0, (struct sockaddr *)&(udp->server), sizeof(udp->server));
}

int udp_receive(struct udp_conn *udp, char *buf, int len)
{
	int res = recvfrom(udp->sock, buf, len, 0, (struct sockaddr *)&(udp->client), &(udp->client_len));

	return res;
}

void udp_close(struct udp_conn *udp)
{
	close(udp->sock);
	return;
}

int clock_nanosleep(struct timespec *next)
{
	struct timespec now;
	struct timespec sleep;

	// get current time
	clock_gettime(CLOCK_REALTIME, &now);

	// find the time the function should sleep
	sleep.tv_sec = next->tv_sec - now.tv_sec;
	sleep.tv_nsec = next->tv_nsec - now.tv_nsec;

	// if the nanosecon is below zero, decrement the seconds
	if (sleep.tv_nsec < 0)
	{
		sleep.tv_nsec += 1000000000;
		sleep.tv_sec -= 1;
	}

	// sleep
	nanosleep(&sleep, NULL);

	return 0;
}

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec -= 1000000000;
		t->tv_sec += 1;
	}
}

int init_sems_and_mutexes(){
	int res = pthread_mutex_init(&send_lock, NULL);
	int res2 = sem_init(&y_value_received, 0, 0);
	int res3 = sem_init(&new_u, 0, 0);
	int res4 = pthread_mutex_init(&y_mut, NULL);
	int res5 = pthread_mutex_init(&u_mut, NULL);
	int res6 = sem_init(&new_signal, 0, 0 );
	if (res == -1 || res2 == -1 || res3 == -1 || res4 == -1 || res5 == -1 || res6 == -1){
		printf("ERROR INIT\n");
		return -1;
	}
	return 0;
}

int destroy_sems_and_mutexes(){

	int res = pthread_mutex_destroy(&send_lock);
	int res2 = sem_destroy(&y_value_received);
	int res3 = sem_destroy(&new_u);
	int res4 = pthread_mutex_destroy(&y_mut);
	int res5 = pthread_mutex_destroy(&u_mut);
	int res6 = sem_destroy(&new_signal);
	if (res == -1 || res2 == -1 || res3 == -1|| res4 == -1 || res5 == -1 || res6 == -1){
		printf("ERROR destroying\n");
		return -1;
	}
	return 0;
}

int main(){

	struct udp_conn udp_client;
	udp_init_client(&udp_client, 9999, "192.168.0.1");
	init_sems_and_mutexes();

	struct thread_args *args = malloc(sizeof *args);
	(args->u) = 0;
	(args-> y) = 0;
	(args-> udp) = udp_client;

	pthread_t listner;
	pthread_create(&listner, NULL, udp_listner, args);
	pthread_t PI;
	pthread_create(&PI, NULL, PI_controller, args);
	pthread_t sim;
	pthread_create(&sim, NULL, simulate, args);
	pthread_t stop_sim;
	pthread_create(&stop_sim, NULL, stop_simulation, args);
	pthread_t reply_signal;
	pthread_create(&reply_signal, NULL, reply_to_signal, args);
	
	pthread_join(listner, NULL);
	pthread_join(sim, NULL);
	pthread_join(stop_sim, NULL);
	pthread_join(PI, NULL);
	pthread_join(reply_signal, NULL);
	free(args);

	udp_close(&udp_client);
	destroy_sems_and_mutexes();

	return 0;
}

