#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

const int N_THREADS = 2;

void norm_min_max_and_classify_parallel(int v[], size_t v_sz, int nThreads){
//void norm_min_max_and_classify_parallel(int v[], size_t v_sz){

//Criar N_THREADS



//Dividir o trabalho pelas N_THREADS




//As N_THREADS colaboram entre si para descobrir o min e máx global

int max = 50;
int min = 10;


//"transformar" o vetor em um de 0 a 100 e calcular a média
int sum = 0;
for(int i = 0; i < v_sz ; i++){

    v[i] = ((v[i] - min) * 100) / (max-min);
    sum += v[i];

}

//DEBUG
for(int i = 0; i < v_sz ; i++){
    printf("%d,",v[i]);

}


int media = sum/v_sz;

//DEBUG
printf("\nmedia = %d\n", media);

//atribuir o valor 1 se o valor for maior ou igual que a média e 0 caso contrário 
for(int i = 0; i < v_sz ; i++){

    if(v[i] >= media) v[i] = 1;
    else v[i] = 0;

}

//DEBUG
for(int i = 0; i < v_sz ; i++){
    printf("%d,",v[i]);

}
    
}

int main(){

int v[] = {10,20,30,40,50};

int size = sizeof (v) / sizeof (int);

//DEBUG
printf ("\nsize = %d\n", size);

}
