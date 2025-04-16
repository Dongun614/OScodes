#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(){
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];

    pid_t pid;
    int fd[2];

    if(pipe(fd) == -1){
        fprintf(stderr, "Piped Failed\n");
        return 1;
    }

    pid = fork();

    if(pid < 0){
        frpintf(stderr, "Fork failed\n");
        return 1;
    }

    if(pid > 0){
        close(fd[READ_END]);

        write(fd[WRITE_END], write_msg, strlen(write_msg)+1);

        close(fd[WRITE_END]);
    }
    else{
        close(fd[WRITE_END]);

        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Child read %s\n", read_msg);

        close(fd[READ_END]);
    }

    return 0;
}