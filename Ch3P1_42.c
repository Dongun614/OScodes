//Zombies: Example 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    if(fork() == 0){
        //child
        printf("Terminating Child, PID = %d\n", getpid());
        exit(0);
    } else{
        printf("Running Parent, PID = %d\n", getpid());
        while(1); // infinite loop
    }

    return 0;
}