#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 2
#define GLOBAL_MIN_INIT_VALUE -1
#define GLOBAL_MAX_INIT_VALUE -1

#define DEBUG

typedef struct{
    int * values;

    int id;

    size_t lim_inf;
    size_t lim_sup;

    int * global_min;
    int * global_max;

    pthread_mutex_t * mutex;
    pthread_barrier_t * barrier;

    int * somatorio;

    size_t values_size;

} thArg;

void * thread_func(void * arg){
    thArg * args = (thArg *)arg;
    int local_min = args->values[args->lim_inf];
    int local_max = args->values[args->lim_sup];
    int media;

    for(size_t i = args->lim_inf; i <= args->lim_sup; i++){
        if(args->values[i] < local_min)
            local_min = args->values[i];

        if(args->values[i] > local_max)
            local_max = args->values[i];
    }

    #ifdef DEBUG
    printf("Thread %d, mínimo local = %d, máximo local = %d\n", args->id, local_min, local_max);
    #endif

    pthread_mutex_lock(args->mutex);
    if(local_min < *(args->global_min))
        *(args->global_min) = local_min;

    if(local_max > *(args->global_max))
        *(args->global_max) = local_max;
    pthread_mutex_unlock(args->mutex);

    #ifdef DEBUG
    printf("Thread %d atualizou valores min e/ou max globais!\n", args->id);
    #endif

    pthread_barrier_wait(args->barrier);

    //Fase 2 - Normalização do Vetor

    for(size_t i = args->lim_inf; i <= args->lim_sup; i++){
        args->values[i] = (args->values[i] - *args->global_min) * 100 / (*args->global_max - *args->global_min);

        *args->somatorio += args->values[i];
        #ifdef DEBUG
        printf("%d adicionado ao somatório!\n", args->values[i]);
        #endif
    }

    pthread_barrier_wait(args->barrier);
    media = *args->somatorio/args->values_size;

    #ifdef DEBUG
    printf("Média calculada pela thread %d é %d\n", args->id, media);
    #endif

    for(size_t i = args->lim_inf; i <= args->lim_sup; i++){
        if(args->values[i] < media)
            args->values[i] = 0;
        else
            args->values[i] = 1;
    }

    

}

void norm_min_max_and_classify_parallel(int v[], size_t v_sz, int nThreads){
    pthread_t threads[N_THREADS];
    thArg arg[N_THREADS];

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, N_THREADS);

    int somatorio = 0;


    int global_min = v[0];
    int global_max = v[0];

    size_t nTermosThread = v_sz / N_THREADS;
    size_t nTermosResto = v_sz % N_THREADS;

    #ifdef DEBUG
    printf("Termos por thread: %ld\nTermos resto: %ld\n", nTermosThread, nTermosResto);
    #endif

    for(int i = 0; i < N_THREADS; i++){
        #ifdef DEBUG
        printf("Inicialização dos args de %d\n", i);
        #endif

        arg[i].values = v;
        arg[i].lim_inf = i*nTermosThread;
        arg[i].lim_sup = (arg[i].lim_inf) + nTermosThread - 1;
        arg[i].id = i;
        arg[i].mutex = &mutex;
        arg[i].global_min = &global_min;
        arg[i].global_max = &global_max;
        arg[i].barrier = &barrier;
        arg[i].somatorio = &somatorio;
        arg[i].values_size = v_sz;

        if(i == N_THREADS - 1)
            arg[i].lim_sup += nTermosResto;

        #ifdef DEBUG
        printf("Thread na posição %d, com limite inferior %ld e limite superior %ld\n", i, arg[i].lim_inf, arg[i].lim_sup);
        #endif
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_func, &arg[i]);
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    #ifdef DEBUG
    printf("Mínimo global: %d e máximo global: %d\n", global_min, global_max);
    #endif

    #ifdef DEBUG
    printf("Vetor values: ");
    for(size_t i = 0; i < v_sz; i++)
        printf("%d ",v[i]);
    printf("\n");
    #endif

    //MUTEX AND BARRIER DESTROY AQUI! (fim do código)
}

int main(){
    int v[] = {10, 20, 30, 40, 50};
    norm_min_max_and_classify_parallel(v, 5, N_THREADS);
}
