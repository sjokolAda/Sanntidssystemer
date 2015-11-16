
#include "shared.h"
#include "signalHandler.h"

void* signalHandler()
{
	int err;
	while(1){
	
		sem_wait(&new_sig);
		// send ack
		pthread_mutex_lock(&tinfo_connection.sendlock);
		err = udp_send(&tinfo_connection.conn, "SIGNAL_ACK", 11);
		if (err==-1){
			printf("Failed to send ack\n");	
		}
		pthread_mutex_unlock(&tinfo_connection.sendlock);
	}
}
