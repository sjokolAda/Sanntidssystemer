#include "shared.h" 
#include "PIDctrl.h"
#include <math.h>
#include <time.h>

//PID controller
void* PIDctrl()
{	
	float y_upd = 0;
	float u = 0;
	char buf[MSG_LENGTH];

	//Init the PID struct parameters
	pid_param_t PI ={
		.Kp = K_P,
		.Ki = K_I,
		.integral = 0.0,
		.error = INIT_ERR,
		.reference = INIT_REF,
		.period = PERIOD,
	};	

	struct timespec period;
	clock_gettime(CLOCK_REALTIME, &period);
	int i = 0;

	while(1){	
		i++;
		timespec_add_us(&period, (PERIOD*1000000));

		pthread_mutex_lock(&tinfo_connection.sendlock);
		if((udp_send(&tinfo_connection.conn, "GET", 4))==-1)
		{
			printf("Failed GET PIDcntrl\n");	
			exit(1);
		}
		pthread_mutex_unlock(&tinfo_connection.sendlock);

		//Signal new value is ready 
		sem_wait(&y_updt);
			
		pthread_mutex_lock(&tinfo_output.lock);
		y_upd = tinfo_output.y_val;
		pthread_mutex_unlock(&tinfo_output.lock);
		
		//Calculate output value
		PI.error = PI.reference - y_upd;
	    	PI.integral = PI.integral + (PI.error * PI.period);
	    	u = PI.Kp * PI.error + PI.Ki * PI.integral;

		//Add output value to buffer
		sprintf(buf, "SET:%f", u); 

		//Send bufferen to server	
		pthread_mutex_lock(&tinfo_connection.sendlock);
		if((udp_send(&tinfo_connection.conn, buf,sizeof(buf)))==-1)
		{
			perror("Failed SET\n");	
		}
		pthread_mutex_unlock(&tinfo_connection.sendlock);
		if((i*PERIOD) > RUNTIME)
		{	
			break;
		}
	
		//Sleeep until new period zzzzz
		clock_nanosleep1(&period);	 				
	}
	return NULL;
}


