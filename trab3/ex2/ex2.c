#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define DEBUG

#define ACCOUNTS_MAX 10
#define ACCOUNT_BALANCE 1000
#define N_THREADS 2
#define NUM_TRANSFERS_PER_THREAD 100
#define MAX_TRANSFER_VALUE 100




typedef struct{
    int id;
    double balance;
    pthread_mutex_t mutex;
}account_t;


int accounts_init(account_t * account_array, int n_accounts){
    if(account_array == NULL || n_accounts <= 0) return 0;

    #ifdef DEBUG
        double total_inicial_debug = 0;
    #endif

    for(int i = 0; i < n_accounts; i++){
        account_array[i].id = i;
        account_array[i].balance = ACCOUNT_BALANCE;
        pthread_mutex_init(&account_array[i].mutex, NULL); 

        #ifdef DEBUG
            printf("Conta %d iniciada\n", account_array[i].id);
            total_inicial_debug += account_array[i].balance;
        #endif
    }

    #ifdef DEBUG
        printf("Valor total das contas (inicial): %lf\n", total_inicial_debug);
    #endif

    return 1;
}

int transfer(account_t * account_array, int source_id, int dest_id, double value){
    if(account_array[source_id].balance < value){

        #ifdef DEBUG
            printf("Transferência sem sucesso de %d para %d de valor %lf", source_id, dest_id, value);
        #endif

        return 0;
    }
    
    //Início da zona de exclusão
    pthread_mutex_lock(&account_array[source_id].mutex);
    pthread_mutex_lock(&account_array[dest_id].mutex);

    account_array[source_id].balance -= value;
    account_array[dest_id].balance += value;

    pthread_mutex_unlock(&account_array[dest_id].mutex);
    pthread_mutex_unlock(&account_array[source_id].mutex);
    //Fim da zona de exclusão


    #ifdef DEBUG
        printf("Transferência de %d para %d com valor %lf\n", source_id, dest_id, value);
    #endif

    return 1;
}


void * thread_function(void * arg){
    account_t * contas = (account_t *)arg;
    int transfer_counter = 0;
    int dst = -1;
    int src = -1;
    double value = -1;

    unsigned int seed = time(NULL) * pthread_self();
    printf("%ud\n", seed);

    while(transfer_counter < NUM_TRANSFERS_PER_THREAD){
        dst = rand_r(&seed) % (ACCOUNTS_MAX);
        do{src = rand_r(&seed) % (ACCOUNTS_MAX);
        }while(src == dst);
        
        value = rand() % (MAX_TRANSFER_VALUE);
        transfer(contas, src, dst, value);

        transfer_counter++;
    }

    return NULL;
}


int main(){
    account_t accounts[ACCOUNTS_MAX];
    accounts_init(accounts, ACCOUNTS_MAX);

    pthread_t threads[N_THREADS];

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_function, (void *)accounts);
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    #ifdef DEBUG
        double total_final_debug = 0;
        for(int i = 0; i < ACCOUNTS_MAX; i++){
            total_final_debug += accounts[i].balance;
            printf("Valor na conta %d: %lf\n",accounts[i].id, accounts[i].balance);
        }
        printf("Valor total das contas (final): %lf\n", total_final_debug);
    #endif
}
