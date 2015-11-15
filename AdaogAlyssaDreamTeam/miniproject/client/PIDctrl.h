#ifndef PID_CTRL_H_
#define PID_CTRL_H_

//Constants for PID controller
#define K_P 10
#define K_I 800
#define PERIOD 0.005
#define INIT_ERR 0
#define	INIT_REF 1.0
#define RUNTIME 0.5

//PID struct parameters
typedef struct
{
	float Kp;
	float Ki;
	float integral;
	float error;
	float reference;
	float period;
}pid_param_t;

void* PIDctrl();


#endif /* PID_CTRL_H_ */
