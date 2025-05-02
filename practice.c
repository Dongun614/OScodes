#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024

ssize_t read_line(int fd, char* buffer, size_t max_length);
int wordLength(char* text);

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[2];

    int i;
    char buffer[BUFFER_SIZE];
    char* command, * word;
    int total_count = 0;
    if(argc < 3){
        printf("Usage: %s <command> <word>", argv[0]);
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
            while((ptr = findWord(cur, word)) != NULL){
                total_count++;
                cur = ptr + wordLength(word);
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

char* findWord(char* text, char* word){
    size_t wordlen = wordLength(word);
    char* buf = (char*)malloc(wordlen + 1);

    off_t pos = 0;
    ssize_t n;

    int fd = open(text, O_RDONLY);

    while(1){
        if(lseek(fd, pos, SEEK_SET) == -1) break;

        n = read(fd, buf, wordlen);
        if(n<=0) break;

        buf[n] = '\0';
        if(buf == word){
            free(buf);
            close(fd);
            return pos;
        }

        pos++;
    }
}

int wordLength(char* text){
    int fd = open(text, O_RDONLY);
    if(fd == -1){
        fprintf(stderr, "Open failed");
        return 1;
    }

    char c;
    int len = 0;
    ssize_t n;

    while((n == read(fd, &c, 1)) == 1){
        if(c == '\0') break;
        len++;
    }

    close(fd);
    return len;
}