#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(){
    int fd;
    char* myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);
    char arr1[80], arr2[80]; //2를 쓰는 공간

    while(1){
        fd = open(myfifo, O_WRONLY);
        fgets(arr2, 80, stdin);
        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        fd = open(myfifo, O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        printf("User 2: %s\n", arr1);
        close(fd);
    }

    return 0;
}