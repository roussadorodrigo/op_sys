#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_WORKS 5
#define MAX_THREADS 2

typedef void *(*work_func)(void *);

typedef struct {
    work_func func;
    void *args;
} work_item_t;


typedef struct {
    work_item_t * works[MAX_WORKS];
    pthread_mutex_t mutex;
    int next_work_position;
} sharedBuffer;

typedef struct {
    sharedBuffer sb;
    pthread_t *workers;
    int n_workers;
} thread_pool_t;

// Shared Buffer

int sharedBuffer_init(sharedBuffer *sb, int queue_dim) {
    if(pthread_mutex_init(&sb->mutex, NULL) != 0)
        return -1;
    sb->next_work_position = 0;
    for(int i = 0; i < queue_dim; i++)
        sb->works[i] = NULL;
    return 0;
}

int sharedBuffer_put(sharedBuffer *sb, work_item_t *wi) {
    if(pthread_mutex_lock(&sb->mutex) != 0)
        return -1;
    if(sb->next_work_position >= MAX_WORKS)
        return -1;

    sb->works[sb->next_work_position] = wi;
    sb->next_work_position++;
    if(pthread_mutex_unlock(&sb->mutex) != 0)
        return -1;
    return 0;
}

work_item_t * sharedBuffer_get(sharedBuffer *sb) {
    if(pthread_mutex_lock(&sb->mutex) != 0)
        return NULL;
    if(sb->works[sb->next_work_position-1] == NULL)
        return NULL;

    work_item_t * aux =  sb->works[sb->next_work_position-1];
    sb->next_work_position--;
    if(pthread_mutex_unlock(&sb->mutex) != 0)
        return NULL;
    return aux;   
}

int sharedBuffer_destroy(sharedBuffer *sb) {
    if(pthread_mutex_destroy(&sb->mutex) != 0)
        return -1;
    return 0;
}

// Work Items
work_item_t *work_item_new(work_func func, void *args) {
    work_item_t *wi = malloc(sizeof(work_item_t));
    if (wi == NULL) return NULL;
    wi->func = func;
    wi->args = args;
    return wi;
}

void work_item_delete(work_item_t *wi) {
    free(wi);
}

// Worker Thread
void *worker_thread(void *arg) {
    thread_pool_t *tp = (thread_pool_t *)arg;
    while(1){
        work_item_t *wi = sharedBuffer_get(&tp->sb);
        if (wi == NULL) break;
        wi->func(wi->args);
        work_item_delete(wi);
    }
    return NULL;
}

// Thread Pool
int thread_pool_init(thread_pool_t * tp, int queue_dim, int n_threads){
	if(sharedBuffer_init(&tp->sb, queue_dim) == -1)
        return -1;
	tp->n_workers = n_threads;
	tp->workers = malloc(sizeof(pthread_t) * n_threads);
    if(tp->workers == NULL){
        return -1;
    }
	for(int i = 0; i < n_threads; i++){
		if(pthread_create(&tp->workers[i], NULL, worker_thread, tp) != 0)
            return -1;
    }
    return 0;
}

int thread_pool_submit(thread_pool_t *tp, work_func func, void *args) {
    work_item_t *wi = work_item_new(func, args);
    if(sharedBuffer_put(&tp->sb, wi) == -1)
        return -1;
    return 0;
}

int thread_pool_destroy(thread_pool_t *tp) {
    sharedBuffer_destroy(&tp->sb);
    for (int i = 0; i < tp->n_workers; i++)
        pthread_join(tp->workers[i], NULL);
    free(tp->workers);
    return 0;
}




void * ola(void *arg) {
    int id = *(int *)arg;
    printf("Olá! (Thread %d)\n", id);
    sleep(1);
    return NULL;
}


int main() {
    thread_pool_t pool;

    thread_pool_init(&pool, MAX_WORKS, MAX_THREADS);

    for (int i = 0; i < 10; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        thread_pool_submit(&pool, ola, id);
    }

    sleep(5);
    thread_pool_destroy(&pool);
    printf("Pool destruída\n");
    return 0;
}
