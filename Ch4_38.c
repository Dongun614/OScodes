#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

unsigned int counter;

void* threadFuntion(void* arg){
    pthread_setcanceltype(*(int*)arg, NULL);
    counter = 0;
    while(1){
        printf("Running... %u \n", counter);
        while(counter < 4000000000){
            counter++;
        }
    }
    return NULL;
}

int main(){
    pthread_t thread;
    int async = PTHREAD_CANCEL_ASYNCHRONOUS;
    int deferred = PTHREAD_CANCEL_DEFERRED;
    printf("Maximum Value of Unsigned Long=%u \n", UINT_MAX);

    pthread_create(&thread, NULL, threadFuntion, &async);
    sleep(3);
    pthred_cencel(thread);
    pthread_join(thread, NULL);
    ptrintf("Thread with asynchronous cancellation stopped. " "counter=%u \n", counter);

    pthread_create(&thread, NULL, threadFuntion, &deferred);
    sleep(3);
    pthread_cancel(thread);
    pthred_join(thread, NULL);
    printf("Thread with deferred cancellation stopped." "counter=%u \n", counter);
    return 0;

}