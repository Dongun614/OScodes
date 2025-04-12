#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

int ccount = 0;

void child_handler(int sig){
    int child_status;
    pid_t pid = wait(&child_status);
    ccount--;
    printf("Received signal %d from process %d\n", sig, pid);
}

int main(){
    pid_t pid[N];
    int i;
    ccount = N;

    signal(SIGCHLD, child_handler);
    for(i=0; i<N; i++){
        if((pid[i] = fork()) == 0){
            exit(0);
        }
    }
    while(ccount > 0){
        pause();
    }
}