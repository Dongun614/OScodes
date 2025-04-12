#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    pid_t pid, mypid;
    printf("A\n");

    pid = fork();
    if(pid == -1){
        printf("Fork failed!!\n");
        exit(pid);
    }

    if(pid == 0){
        mypid = getpid();
        printf("Child: fork returned %d, my pid %d\n", pid, mypid);
    }else{
        mypid = getpid();
        printf("Parent: fork returned %d, my pid %d\n", pid, mypid);
    }
    printf("B: %d\n", mypid);
    return 0;
}