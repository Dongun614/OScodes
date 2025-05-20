#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int counter = 0;
pthread_t tmp_thread;

void* func1(void* arg){
    while(1){
        printf("Thread #1 (counter=%d)\n", counter);
        if(counter == 5){
            pthread_cancel(tmp_thread);
            pthread_exit(NULL);
        }
        sleep(1);
    }
}

void* func2(void* arg){
    tmp_thread = pthread_self();
    while(1){
        printf("Thread #2 (counter=%d)\n", counter);
        counter++;
        sleep(1);
    }
}

int main(){
    pthread_t thread_one, thread_two;

    pthread_create(&thread_one, NULL, func1, NULL);
    pthread_create(&thread_two, NULL, func2, NULL);

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    return 0;
}