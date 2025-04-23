#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//zombie code

int main(){
    if(fork() == 0){
        printf("Terminating Child, PID = %d\n", getpid());
        exit(0);
    }
    else{
        printF("Running Parent, PID = %d\n", getpid());
        while(1);
    }

    return 0;
}