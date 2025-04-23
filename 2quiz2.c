#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024

ssize_t read_line(int fd, char* buffer, size_t max_length);

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[2];

    int i;
    char buffer[BUFFER_SIZE];
    char* command, * word;
    int total_count = 0;

    if(argc < 3){
        printf("Usage: %s <command> <word> \n", argv[0]);
        return 0;
    }

    command = argv[1];
    word = argv[2];

    if(pipe(fd) == -1){
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if(pid > 0){
        close(fd[WRITE_END]);
        i=1;
        char* ptr, *cur;
        while(read_line(fd[READ_END], buffer, BUFFER_SIZE-1) != 0){
            cur = buffer;
            while((ptr = strstr(cur, word)) != NULL){
                total_count++;
                cur = ptr + strlen(word);
            }
            i++;
        }

        close(fd[READ_END]);
        wait(NULL);
        printf("Result for '%s' : %d\n", word, total_count);
    }
    else{
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);

        execl("/bin/sh", "sh", "-c", command, NULL);
        exit(1);
    }

    return 0;
}

ssize_t read_line(int fd, char* buffer, size_t max_length){
    ssize_t num_read = 0;
    char c;
    while(read(fd, &c, 1) == 1 && num_read < max_length){
        buffer[num_read++] = c;
        if(c == '\n'){
            break;
        }
    }
    buffer[num_read] = '\0';
    return num_read;
}