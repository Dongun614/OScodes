#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <getopt.h>  
#include <ctype.h>   

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024
#define AC_RED "\x1b[31m"
#define AC_NORMAL "\x1b[m"

int numberONLY = 0;  // -n
int fileNameONLY = 0;    // -l
int matchCountONLY = 0;   // -c
int caseInsensitive = 0;   // -i

ssize_t read_line(int fd, char* buffer, size_t max_length);
int wordLength(char* text);
char* findWord(char* text, char* word, int case_insensitive);

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[2];
    int i;
    char buffer[BUFFER_SIZE];
    char* command, * word;
    int totalCount = 0;
    int opt;
    int showFileName = 0; 

    while ((opt = getopt(argc, argv, "nlci")) != -1) {
        switch (opt) {
            case 'n': numberONLY = 1; break;
            case 'l': fileNameONLY = 1; break;
            case 'c': matchCountONLY = 1; break;
            case 'i': caseInsensitive = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-n] [-l] [-c] [-i] <command> <word>\n", argv[0]);
                return 1;
        }
    }

    if (optind + 1 >= argc) {
        fprintf(stderr, "Usage: %s [-n] [-l] [-c] [-i] <command> <word>\n", argv[0]);
        return 1;
    }

    command = argv[optind];
    word = argv[optind + 1];

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
        char *pCur, *pPtr;
        
        while(read_line(fd[READ_END], buffer, BUFFER_SIZE-1) != 0){
            cur = buffer;
            int matchedLine = 0;
            
            while((ptr = findWord(cur, word, caseInsensitive)) != NULL){
                cur = ptr + wordLength(word);
                matchedLine++;
                totalCount++;
            }
            
            if(matchedLine > 0){
                if(fileNameONLY){
                    if(!showFileName){
                        printf("%s\n", command);
                        showFileName = 1;
                    }
                }
                else if(numberONLY){
                    printf("%d\n", i);
                }
                else if(!matchCountONLY){
                    pCur = buffer;
                    printf("[%d] ", i);
                    while((pPtr = findWord(pCur, word, caseInsensitive)) != NULL){
                        printf("%.*s", (int)(pPtr - pCur), pCur);
                        printf("%s%.*s%s", AC_RED, wordLength(word), pPtr, AC_NORMAL);
                        pCur = pPtr + wordLength(word);
                    }
                    printf("%s", pCur);
                }
            }
            i++;
        }
        
        if(matchCountONLY){
            printf("%d\n", totalCount);
        }
        
        if(totalCount == 0 && !matchCountONLY && !fileNameONLY){
            printf("There is no '%s' in process '%s'\n", word, command);
        }

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

char* findWord(char* text, char* word, int case_insensitive) {
    if (*word == '\0') return text;
    for (char* ret_ptr = text; *ret_ptr; ret_ptr++) {
        char* t = ret_ptr;
        char* w = word;
        while (*t && *w) {
            if (case_insensitive) {
                if (tolower((unsigned char)*t) != tolower((unsigned char)*w)) break;
            } else {
                if (*t != *w) break;
            }
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
