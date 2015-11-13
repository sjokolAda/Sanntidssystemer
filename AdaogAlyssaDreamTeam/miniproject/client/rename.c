



#define K_P = 10;
#define K_I = 800;


int pi_controller()
{
    error = reference -y;
    integral = integral + (error*period);
    U = K_P * error + K_I * integral;
    
}


