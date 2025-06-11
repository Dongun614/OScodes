#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct _rwlock_t {
    sem_t mutex;
    sem_t writelock;
    sem_t read_try;
    int read_count;
    int write_waiting;
} rwlock_t;

typedef struct {
    int id;
    int time;
    char type;
} thread_node;

rwlock_t rw;
int thread_counter = 0;
int reader_counter = 1;
int writer_counter = 1;
pthread_t threads[100];

// 타임스탬프 출력 함수
void print_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("[%.4f] ", tv.tv_sec + tv.tv_usec / 1000000.0);
}

// RWLock 초기화
void rwlock_init(rwlock_t *rw) {
    rw->read_count = 0;
    rw->write_waiting = 0;
    sem_init(&rw->mutex, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->read_try, 0, 1);
}

// Read Lock 획득
void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->read_try);
    sem_wait(&rw->mutex);
    rw->read_count++;
    if (rw->read_count == 1)
        sem_wait(&rw->writelock);
    sem_post(&rw->mutex);
    sem_post(&rw->read_try);
}

// Read Lock 해제
void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->mutex);
    rw->read_count--;
    if (rw->read_count == 0)
        sem_post(&rw->writelock);
    sem_post(&rw->mutex);
}

// Write Lock 획득
void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->mutex);
    rw->write_waiting++;
    if (rw->write_waiting == 1)
        sem_wait(&rw->read_try);
    sem_post(&rw->mutex);

    sem_wait(&rw->writelock);
}

// Write Lock 해제
void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->writelock);

    sem_wait(&rw->mutex);
    rw->write_waiting--;
    if (rw->write_waiting == 0)
        sem_post(&rw->read_try);
    sem_post(&rw->mutex);
}

// Reader Thread 함수
void* reader(void* arg) {
    thread_node* t = (thread_node*) arg;
    print_time(); printf("Reader#%d created!\n", t->id);
    rwlock_acquire_readlock(&rw);
    print_time(); printf("Reader#%d Read started! (reading %d ms)\n", t->id, t->time);
    usleep(t->time * 1000);
    print_time(); printf("Reader#%d Terminated!\n", t->id);
    rwlock_release_readlock(&rw);
    free(t);
    return NULL;
}

// Writer Thread 함수
void* writer(void* arg) {
    thread_node* t = (thread_node*) arg;
    print_time(); printf("Writer#%d created!\n", t->id);
    rwlock_acquire_writelock(&rw);
    print_time(); printf("Writer#%d Write started! (writing %d ms)\n", t->id, t->time);
    usleep(t->time * 1000);
    print_time(); printf("Writer#%d Terminated!\n", t->id);
    rwlock_release_writelock(&rw);
    free(t);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sequence file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    rwlock_init(&rw);

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;

        char type;
        int time;
        if (sscanf(line, "%c%d", &type, &time) != 2)
            continue;

        thread_node* arg = malloc(sizeof(thread_node));
        arg->time = time;
        arg->type = type;

        if (type == 'R') {
            arg->id = reader_counter++;
            pthread_create(&threads[thread_counter++], NULL, reader, arg);
        } else if (type == 'W') {
            arg->id = writer_counter++;
            pthread_create(&threads[thread_counter++], NULL, writer, arg);
        }

        usleep(100000); // 100ms 간격
    }

    for (int i = 0; i < thread_counter; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(fp);
    return 0;
}
