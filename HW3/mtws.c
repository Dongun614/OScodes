// mtws.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH 1024

// ====== Structs & Globals ======
typedef struct {
    char **buffer;
    int capacity;
    int front, rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t full, empty;
} Buffer;

Buffer buffer;
char search_word[128];
int total_found = 0;
int finished = 0;
pthread_mutex_t total_mutex;

// ====== Utility Functions ======
void toLower(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

int checkFile(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext && (strcmp(ext, ".txt") == 0);
}

void initBuffer(Buffer *b, int size) {
    b->buffer = malloc(sizeof(char*) * size);
    b->capacity = size;
    b->front = b->rear = b->count = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->full, NULL);
    pthread_cond_init(&b->empty, NULL);
}

// void destroy_buffer(Buffer *b) {
//     free(b->buffer);
//     pthread_mutex_destroy(&b->mutex);
//     pthread_cond_destroy(&b->full);
//     pthread_cond_destroy(&b->empty);
// }

void putBuffer(Buffer *b, const char *path) {
    pthread_mutex_lock(&b->mutex);
    while (b->count == b->capacity)
        pthread_cond_wait(&b->full, &b->mutex);

    b->buffer[b->rear] = strdup(path);
    b->rear = (b->rear + 1) % b->capacity;
    b->count++;

    pthread_cond_signal(&b->empty);
    pthread_mutex_unlock(&b->mutex);
}

char* get_buffer(Buffer *b) {
    pthread_mutex_lock(&b->mutex);
    while (b->count == 0) {
        if (finished) {
            pthread_mutex_unlock(&b->mutex);
            return NULL;
        }
        pthread_cond_wait(&b->empty, &b->mutex);
    }

    char *path = b->buffer[b->front];
    b->front = (b->front + 1) % b->capacity;
    b->count--;

    pthread_cond_signal(&b->full);
    pthread_mutex_unlock(&b->mutex);
    return path;
}

// 프로토타입 선언
void recursive_search(const char *path);

// producer_thread 함수
void* producer_thread(void *arg) {
    char *dir_path = (char*)arg;
    recursive_search(dir_path);

    pthread_mutex_lock(&buffer.mutex);
    finished = 1;
    pthread_cond_broadcast(&buffer.empty);
    pthread_mutex_unlock(&buffer.mutex);
    return NULL;
}

// 실제 정의는 함수 밖에서
void recursive_search(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (!dp) return;

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);
        struct stat st;
        stat(full_path, &st);

        if (S_ISDIR(st.st_mode)) {
            recursive_search(full_path);
        } else if (S_ISREG(st.st_mode) && checkFile(entry->d_name)) {
            putBuffer(&buffer, full_path);
        }
    }
    closedir(dp);
}

int count_word_in_file(const char *filepath, const char *word) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return 0;
    int count = 0;
    char buf[1024];

    while (fscanf(fp, " %1023s", buf) == 1) {
        char lower_buf[1024];
        strncpy(lower_buf, buf, sizeof(lower_buf));
        toLower(lower_buf);
        if (strcmp(lower_buf, word) == 0)
            count++;
    }

    fclose(fp);
    return count;
}

void* consumer_thread(void *arg) {
    while (1) {
        char *file_path = get_buffer(&buffer);
        if (!file_path) break;

        int found = count_word_in_file(file_path, search_word);
        pthread_mutex_lock(&total_mutex);
        total_found += found;
        pthread_mutex_unlock(&total_mutex);

        printf("[Thread %ld] %s: %d\n", pthread_self(), file_path, found);
        free(file_path);
    }
    return NULL;
}

// ====== Main ======
int main(int argc, char *argv[]) {
    int opt, buf_size = 0, thread_num = 0;
    char *dir = NULL;

    while ((opt = getopt(argc, argv, "b:t:d:w:")) != -1) {
        switch (opt) {
            case 'b': buf_size = atoi(optarg); break;
            case 't': thread_num = atoi(optarg); break;
            case 'd': dir = optarg; break;
            case 'w': strncpy(search_word, optarg, sizeof(search_word)); break;
            default:
                fprintf(stderr, "Usage: %s -b <buffer> -t <threads> -d <dir> -w <word>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!buf_size || !thread_num || !dir || !search_word[0]) {
        fprintf(stderr, "Missing required arguments.\n");
        exit(EXIT_FAILURE);
    }

    toLower(search_word);
    initBuffer(&buffer, buf_size);
    pthread_mutex_init(&total_mutex, NULL);

    pthread_t prod;
    pthread_create(&prod, NULL, producer_thread, dir);

    pthread_t *threads = malloc(sizeof(pthread_t) * thread_num);
    for (int i = 0; i < thread_num; ++i)
        pthread_create(&threads[i], NULL, consumer_thread, NULL);

    pthread_join(prod, NULL);
    for (int i = 0; i < thread_num; ++i)
        pthread_join(threads[i], NULL);

    printf("Total words found: %d\n", total_found);

    destroy_buffer(&buffer);
    pthread_mutex_destroy(&total_mutex);
    free(threads);
    return 0;
}
