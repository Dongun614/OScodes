#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024
#define AC_RED "\x1b[31m"
#define AC_NORMAL "\x1b[m"

ssize_t read_line(int fd, char* buffer, size_t max_length);
int wordLength(char* text);
char* findWord(char* text, char* word);

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
        char *ptr, *cur;
        char *p_cur, *p_ptr;
        while(read_line(fd[READ_END], buffer, BUFFER_SIZE-1) != 0){
            cur = buffer;
            int print_flag = 0;
            while((ptr = findWord(cur, word)) != NULL){
                cur = ptr + wordLength(word);
                print_flag = 1;
                total_count++;
            }
            if(print_flag){
                printf("[%d] ", i);
                p_cur = buffer;
                while((p_ptr = findWord(p_cur, word)) != NULL){
                    printf("%.*s", (int)(p_ptr - p_cur), p_cur);
                    printf(AC_RED "%.*s" AC_NORMAL, wordLength(word), p_ptr);
                    p_cur = p_ptr + wordLength(word);
                }
                printf("%s", p_cur);
            }
            i++;
        }
// 146 148 152 152 157 159 164 168 169 176 178 182 184 188 189 190 219 250
        if(total_count == 0){
            printf("There is no '%s' in process '%s'\n", word, command);
        }

        //printf("total count : %d\n", total_count);

        close(fd[READ_END]);
        wait(NULL);
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

char* findWord(char* text, char* word) {
    if (*word == '\0') return text;
    for (char* ret_ptr = text; *ret_ptr; ret_ptr++) {
        char* t = ret_ptr;
        char* w = word;
        while (*t && *w && *t == *w) {
            t++;
            w++;
        }
        if (*w == '\0') return ret_ptr;
    }
    return NULL;
}

int wordLength(char* text) {
    int len = 0;
    while (text[len] != '\0') {
        len++;
    }
    return len;
}