#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N_THREADS 5

typedef struct{
	pthread_mutex_t * mutex;
	pthread_cond_t * cond;
	int max_threads; //N
	int waiting_threads; //n
	
}sot_barrier_t;

int sot_barrier_init(sot_barrier_t * barrier, int numberOfThreads){
	
	//INTI MUTEX
	pthread_mutex_init(barrier->mutex, NULL);
	
	//INIT COND
	pthread_cond_init(barrier->cond, NULL);
	
	barrier->max_threads = numberOfThreads;
	barrier->waiting_threads = 0;
	
	return 1;
}

int sot_barrier_destroy(sot_barrier_t * barrier){
	
	//DESTROY MUTEX
	pthread_mutex_destroy(barrier->mutex);
	
	//DESTROY COND
	pthread_cond_destroy(barrier->cond);
	return 1;
	
	
}

int sot_barrier_wait(sot_barrier_t * barrier){ 
	
	pthread_mutex_lock(barrier->mutex);
	
	barrier->waiting_threads++;
	
	if(barrier->waiting_threads == barrier->max_threads){
		
		barrier->waiting_threads = 0;
		pthread_cond_broadcast(barrier->cond);
		
	}else{
		while(barrier->waiting_threads > 0){
			pthread_cond_wait(barrier->cond, barrier->mutex);
			
			
		}
	}
	
	
	
	pthread_mutex_unlock(barrier->mutex);
	return 1;
	
}


void* thread_func(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d: before barrier\n", id);
    
    // Wait at the barrier
    sot_barrier_wait(&my_barrier);

    printf("Thread %d: after barrier\n", id);
    free(arg);
    return NULL;
}


int main() {
    pthread_t threads[N_THREADS];
    
    sot_barrier_t my_barrier;

    // Initialize the barrier
    sot_barrier_init(&my_barrier, N_THREADS);

    for (int i = 0; i < N_THREADS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, thread_func, arg);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    sot_barrier_destroy(&my_barrier);

    return 0;
}


