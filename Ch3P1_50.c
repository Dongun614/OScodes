#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
    char *argv[2];
    argv[0] = "./hello";
    argv[1] = NULL;

    pid_t pid1;
    pid_t pid2;
    int ret;
    int status;

    printf("A\n");

    pid1 = fork();
    if(pid1 == 0){
        printf("B\n");

        pid2 = fork();
            if(pid2 == 0){
                printf("C\n");
                execvp("./hello", argv);
            } else{
                ret = wait(&status);
                printf("D\n");
                exit(0);
            }
        } else{
            printf("E\n");
            ret = wait(&status);
            printf("F\n");
        }

        return 0;
    }