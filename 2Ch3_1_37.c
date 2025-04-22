#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    pid_t pid;
    int ret;
    char* argv[2];

    argv[0] = "./hello";
    argv[1] = NULL;

    pid = fork();
    if (pid == 0){
        ret = execvp("./hello", argv);
        if(ret < 0){
            perror("Error: execvp failed");
            exit(EXIT_FAILURE);
        }
    }
    else if(pid > 0){
        wait(NULL);
    }
    else {
        perror("Error: fork failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}