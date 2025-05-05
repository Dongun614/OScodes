#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "findword.h"

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
