#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

const int N_THREADS = 2;

void norm_min_max_and_classify_parallel(int v[], size_t v_sz, int nThreads){
//Criar N_THREADS




//Dividir o trabalho pelas N_THREADS




//As N_THREADS colaboram entre si para descobrir o min e máx global




//"transformar" o vetor em um de 0 a 100 e calcular a média

for(int i = 0; i < v_sz ; i++){

    v[i] = (v[i] – min) * 100 / (max-min);

}
//atribuir o valor 1 se o valor for maior ou igual que a média e 0 caso contrário 


}



int main(){

int v[] = {10,20,30,40,50};

int size = sizeof (v) / sizeof (int);

//DEBUG
printf ("size = %d\n", size);


}
