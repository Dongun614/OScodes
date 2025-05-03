#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(){
    char line[MAX_LINE];
    char* args[MAX_ARGS];

    while(1){
        printf("mysh> ");
        fflush(stdout);

        if(!fgets(line, MAX_LINE, stdin)){
            perror("fgets failed");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if(strcmp(line, "quit") == 0) break;

        int argc = 0;
        char* token = strtok(line, " ");
        while(token != NULL && argc < MAX_ARGS - 1){
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        pid_t pid = fork();
        if(pid < 0){
            perror("fork faied");
        }
        else if(pid == 0){
            if(execvp(args[0], args) == -1){
                perror("execvp failed");
            }
            exit(1);
        }
        else{
            wait(NULL);//kj
        }
    }

    printf("Shell exited.\n");
    return 0;
}