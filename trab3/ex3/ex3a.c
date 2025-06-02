#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define MAX 10000000
#define MIN 0

typedef struct {
    int *v;
    size_t start;
    size_t end;
    int min_l;
    int max_l;
} ThreadData;

void *min_max_thread(void *nulo) {

//COnverter o void* para um ThreadData*
    ThreadData *dados = (ThreadData *)nulo;

    dados->min_l = MAX;
    dados->max_l = MIN;

    for (size_t i = dados->start; i < dados->end; i++) {
        if (dados->v[i] < dados->min_l) dados->min_l = dados->v[i];
        if (dados->v[i] > dados->max_l) dados->max_l = dados->v[i];
    }

    return NULL;
}


void norm_min_max_and_classify_parallel(int v[], size_t v_sz, int nThreads) {
    pthread_t threads[nThreads];
    ThreadData tdata[nThreads];

//Dividir o trabalho pelas N_THREADS
    size_t bloco = v_sz / nThreads;
    size_t resto_bloco = v_sz % nThreads;
    size_t inicio = 0;
    for (int i = 0; i < nThreads; i++) {
        size_t pedaco = bloco;

//Distribuir o resto pela primeira thread
        if (resto_bloco > 0){
            pedaco++;
            resto_bloco--;
        }

        tdata[i].v = v;
        tdata[i].start = inicio;

        tdata[i].end = inicio + pedaco;

//inicio da proxima thread
        inicio = tdata[i].end;
        
//Criar N_THREADS
        pthread_create(&threads[i], NULL, min_max_thread, &tdata[i]);
    }

    int min_g = MAX;
    int max_g = MIN;

//Ambas as threads "descobrem" qual o max e min global
    for (int i = 0; i < nThreads; i++) {

        pthread_join(threads[i], NULL);
        if (tdata[i].min_l < min_g) min_g = tdata[i].min_l;
        if (tdata[i].max_l > max_g) max_g = tdata[i].max_l;
    }

//"transformar" o vetor em um de 0 a 100 e calcular a média
    int sum = 0;
    int media = 0;
    for (size_t i = 0; i < v_sz; i++) {

        v[i] = (v[i] - min_g) * 100 / (max_g - min_g);

        sum += v[i];
        printf("%d ",v[i]);
    }
    printf("\n");

    media = sum / v_sz;

//atribuir o valor 1 se o valor for maior ou igual que a média e 0 caso contrário 
    for (size_t i = 0; i < v_sz; i++) {

        if(v[i] >= media) v[i] = 1;
        else v[i] = 0;
    }
}
int main() {
    int v[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    size_t size = sizeof(v) / sizeof(int);
    int nThreads = 2; //2 % size;

    //PRINT DO ARRAY INICIAL
    for (size_t i = 0; i < size; i++) {

        printf("%d ", v[i]);
    }
    printf("\n");

    norm_min_max_and_classify_parallel(v, size, nThreads);

    //PRINT DO ARRAY NORMALIZADO
    for (size_t i = 0; i < size; i++) {

        printf("%d ", v[i]);
    }
    printf("\n");

    return 0;
}
