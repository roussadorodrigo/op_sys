#include <stdio.h>
#include <pthreads.h>

#define N_THREADS 2
#define MAX_VALUES 100

typedef struct{
	int * values; //as threads estão a partilhar o mesmo array values
	int lim_inf;
	int lim_sup;
	pthread_barrier_t * barrier; //as threads estão a partilhar a mesma barreira barrier
	int * sumTotal;
	pthread_mutex_t * mutex;
}th_args;


void * th(void * _args){
	th_args * arg = (th_args)_args;
	
	//1a fase
	int sumPartial = sum(arg->values, arg->lim_inf, arg->lim_sup);
	
	pthread_mutex_lock(arg->mutex);
	arg->sumTotal += sumPartial;
	pthread_mutex_unlock(arg->mutex);

	//sincronização (pré-2a fase) - não podem avançar sem todas terem terminado a 1a fase! (mecanismo de barreira)
	//esta barreira também garante que, na 2a fase, todas as threads já colocaram o seu sumPartial no sumTotal!
	pthread_barrier_wait(arg->barrier);
	
	//2a fase
	
	//garantimos que não verificamos o valor enquanto há threads a colocar o valor
	pthread_mutex_lock(arg->mutex);
	int media = arg->sumTotal/MAX_VALUES;
	pthread_mutex_unlock(arg->mutex);
	
	//classifica os valores do array dependendo da média e dos seus valores
	classify(arg->values, arg->inf, arg->sup, media);
}


int sum(int * v, int inf, int sup){
	int aux = 0;
	for(int i = inf; i <= sup; i++){
		aux += v[i];
	}
	return aux;
}


int main(){
	pthread_t th_id[N_THREADS];
	th_args args[N_THREADS];
	
	int values[MAX_VALUES]; //assume-se que o values já foi iniciado! (a zero)
	int chunk = MAX_VALUES/N_THREADS;
	
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, N_THREADS);
	
	int sumTotal = 0;
	
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL); //CONFIRMAR ESTE INIT!
	
	for(int i = 0; i < N_THREADS; i++){
		args[i].values = values[i];
		args[i].lim_inf = i*chunk;
		args[i].lim_sup = (i+1)*chunk - 1;
		args[i].barrier = &barrier;
		args[i].sumTotal = &sumTotal;
		args[i].mutex = &mutex;
	
		pthread_create(&th_id, NULL, th, args[i]);
	}
	
	
}
