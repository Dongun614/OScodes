#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

void int_handler(int sig){
    printf("Process %d received signal %d\n", getpid(), sig);
    exit(0);
}

void main(){
    pid_t pid[N];
    int i;
    int child_status;

    signal(SIGINT, int_handler);
    for(i=0; i<N; i++){
        if((pid[i] = fork()) == 0){
            while(1);
        }
    }

    for(i=0; i<N; i++){
        printf("Killing process %d\n", pid[i]);
        kill(pid[i], SIGINT);
    }

    for(i=0; i<N; i++){
        pid_t wpid = wait(&child_status);
        if(WIFEXITED(child_status)){
            printf("Child %d terminated with exit status %d", wpid, WEXITSTATUS(child_status));
        }
        else{
            printf("Child %d terminated abnormally\n", wpid);
        }
    }
}