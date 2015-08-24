#include <stdio.h>
#include <stdlib.h>
#include "Ex1A.h"

void allocate(int value) { 
	int *ptr = NULL; 
	ptr = malloc(100000000000000000 * sizeof(int)); 
	if(ptr == NULL){  
   			
		perror("Out of memory\n");  
   		exit(-1);  
	}	
	*ptr = value; 
	//printf("test of allocated memory: %i\n", value); 
} 

int main(){
	int i=1;
	while(i){
		allocate(i);
	}
	return 0;
} 

