#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
    pid_t pid = fork();

    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        return 1;
    } else if(pid == 0){
        execlp("/bin/ls", "ls", NULL);
    } else {
        wait(NULL);
        printf("Child Completed\n");
    }

    return 0;
}