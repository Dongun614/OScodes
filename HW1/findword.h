#ifndef FINDWORD_H
#define FINDWORD_H

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024
#define AC_RED "\x1b[31m"
#define AC_NORMAL "\x1b[m"

extern int numberONLY;
extern int fileNameONLY;
extern int matchCountONLY;
extern int caseInsensitive;

ssize_t read_line(int fd, char* buffer, size_t max_length);
int wordLength(char* text);
char* findWord(char* text, char* word, int case_insensitive);

#endif
