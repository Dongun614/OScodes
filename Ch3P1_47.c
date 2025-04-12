//Waiting Example 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
    int child_status;

    if(fork() == 0){
        printf("HC: hello from child\n");
    }else{
        printf("HP: hello from parent\n");
        wait(&child_status);
        printf("CT: child has terminated\n");
    }

    print("Bye\n");
    exit(0);
}