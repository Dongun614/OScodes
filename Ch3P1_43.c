#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    if(fork() == 0){
        //child
        printf("Running Child, PID = %d\n", getpid());
        while(1); //infinite loop
    }else{
        printf("Terminating Parent, PID = %d\n", getpid());
        exit(0);
    }

    return 0;
}