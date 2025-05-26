#include <stdio.h>
#include <pthread.h>

#define DEBUG 1

#define ACCOUNTS_MAX 2
#define ACCOUNT_BALANCE 1000
#define N_THREADS 4
#define NUM_TRANSFERS_PER_THREAD 10




typedef struct{
    int id;
    double balance;
    pthread_mutex_t * mutex;

}account_t;


int accounts_init(account_t * account_array, int n_accounts, pthread_mutex_t * mutex_ptr){
    if(account_array == NULL || n_accounts <= 0) return 0;

    #ifdef DEBUG
        double total_inicial_debug = 0;
    #endif

    for(int i = 0; i < n_accounts; i++){
        account_array[i].id = i;
        account_array[i].balance = ACCOUNT_BALANCE;
        account_array[i].mutex = mutex_ptr;

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
            printf("Transferência sem sucesso de %d para %d de valor %lf", source_id, dest_id, );
        #endif

        return 0;
    }

    #ifdef DEBUG
        printf("Transferência de %d para %d com valor %lf\n", source_id, dest_id, value);
    #endif

    return 1;
}


int main(){
    account_t accounts[ACCOUNTS_MAX];
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    accounts_init(accounts, ACCOUNTS_MAX, &mutex);
    
    #ifdef DEBUG
        double total_final_debug = 0;
        for(int i = 0; i < ACCOUNTS_MAX; i++){
            total_final_debug += accounts[i].balance;
            printf("Valor na conta %d: %lf\n",accounts[i].id, accounts[i].balance);
        }
        printf("Valor total das contas (final): %lf\n", total_final_debug);
    #endif
}
