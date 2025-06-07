/*Nota: Esta versão do código é meramente ilustrativa do problema, está sujeita a deadlocks*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define DEBUG
#define N_FILOSOFOS 5

typedef struct{
    int id;
    pthread_mutex_t * mutex;
}Garfo;

typedef struct{
    Garfo * garfo_direito;
    Garfo * garfo_esquerdo;
    int id;
}ArgsFilosofo;


void * filosofo_func(void * arg){
    ArgsFilosofo * args = (ArgsFilosofo *)arg;
    while(1){
        pthread_mutex_lock(args->garfo_esquerdo->mutex);
        #ifdef DEBUG
        printf("O filósofo %d agarrou o garfo esquerdo\n", args->id);
        #endif

        pthread_mutex_lock(args->garfo_direito->mutex);
        #ifdef DEBUG
        printf("O filósofo %d agarrou o garfo direito\n", args->id);
        #endif

        sleep(1);

        pthread_mutex_unlock(args->garfo_direito->mutex);
        #ifdef DEBUG
        printf("O filósofo %d largou o garfo direito\n", args->id);
        #endif

        pthread_mutex_unlock(args->garfo_esquerdo->mutex);
        #ifdef DEBUG
        printf("O filósofo %d largou o garfo esquerdo\n", args->id);
        #endif
    }
}


int main(){
    Garfo garfos[N_FILOSOFOS];
    pthread_t threads[N_FILOSOFOS];
    ArgsFilosofo args[N_FILOSOFOS];
    pthread_mutex_t mutex[N_FILOSOFOS];

    for(int i = 0; i < N_FILOSOFOS; i++){
        garfos[i].mutex = &(mutex[i]);
        pthread_mutex_init(garfos[i].mutex, NULL);
    }

    for(int i = 0; i < N_FILOSOFOS; i++){
        args[i].id = i;
        args[i].garfo_direito = &(garfos[i]);

        if(i+1 > N_FILOSOFOS - 1)
            args[i].garfo_esquerdo = &(garfos[0]);
        else
            args[i].garfo_esquerdo = &(garfos[i+1]);

        pthread_create(&threads[i], NULL, filosofo_func, &args[i]);

        #ifdef DEBUG
            printf("Filósofo %d inicializado\n", args[i].id);
        #endif
    }

        #ifdef DEBUG
            printf("Todos sentados à mesa\n");
        #endif

        for(int i = 0; i < N_FILOSOFOS; i++)
            pthread_join(threads[i], NULL);
    
}
