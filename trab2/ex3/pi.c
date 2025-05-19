#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define MIN_VALUE 10000000000

typedef struct arg_struct{
    int i;
    unsigned long long n_termos;
    double total_parcial;
}ArgStruct;

/*
FUNÇÃO: leibniz
ARGUMENTOS: 
    k -> número de termos
    n -> número de threads a utilizar
*/
double leibniz(unsigned long long k, unsigned long long n){
    long double total = 0;
    long double a = 0;

    while(k <= n){
        a = (k % 2 == 0) ? -1 : 1;
        total += a/((2*k)-1);
        k++;
    }

    return total*4;
}


/*
FUNÇÃO: thread_function
ARGUMENTOS: 
*/
void * thread_function(void * arg_struct){
    ArgStruct *args = (ArgStruct *)arg_struct;
    unsigned long long start = (args->i * args->n_termos) + 1 ;
    unsigned long long end = (start + args->n_termos) - 1;
    total += leibniz(start, end);
    return NULL;
}


/*
FUNÇÃO: main
ARGUMENTOS: 
    argv[1] -> número de termos
    argv[2] -> número de threads
*/
int main(int argc, char * argv[]){
    double resultado = 0;

    if(argc < 3){
        printf("Argumentos incorretos!\n");
        exit(EXIT_FAILURE);
    }

    unsigned long long n_termos = atoll(argv[1]);
    if(n_termos < MIN_VALUE){
        n_termos = MIN_VALUE;
        printf("Número de termos é inferior ao mínimo! Alterado para este valor!\n");
    }

    unsigned long long n_threads = atoll(argv[2]);
    pthread_t thread_ids[n_threads];
    
    unsigned long long n_termos_cada = n_termos/n_threads;
    unsigned long long resto = n_termos%n_threads;

    ArgStruct args[n_threads];

    for(int i = 0; i < n_threads; i++){
        args[i].i = i;
        if(i == n_threads - 1)
            args[i].i = resto;
        else
            args[i].n_termos = n_termos_cada;

        pthread_create(&thread_ids[i], NULL, thread_function, (void *)&args[i]);

    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(thread_ids[i], NULL);
        resultado+=args[i].total_parcial;
    }

    printf("O valor aproximado de pi é: %lf\n", total);
}
