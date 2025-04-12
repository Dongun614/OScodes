#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int fd;
    int start_offset;
    int read_bytes;
    char filename[FILELEN];
    char buf[BUFSIZE];

    //argv가 제대로 들어왔는지 확인
    if(argc < 3){
        fprintf(stderr, "Usage: %s [filename] [start offset] \n", argv[0]);
        exit(1);
    }

    //set
    strcpy(filename, argv[1]);
    start_offset = atoi(argv[2]);

    //fd 설정
    fd = open(filename, O_RDONLY);
    if(fd == -1){
        perror(filename);
        exit(1);
    }

    //set file position
    lseek(fd, start_offset, SEEK_SET);

    //read file
    while(read_bytes = read(fd, buf, BUFSIZE) > 0){
        write(fd, buf, read_bytes);
    }
    printf("\n\n");

    close(fd);

    return 0;
}