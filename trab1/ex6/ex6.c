#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

#define PIPE_RE 0
#define PIPE_WR 1

#define SIZE_BUFF 32

#define MIN_VALUE 10000000000               // valor minimo de termos

long double part_leibniz(unsigned long long i,unsigned long long n){                      //funcao serie de leibniz + parcial 
    long double total = 0;
    double a = 0;
    for(; i <= n; i++){
        a = (i % 2 == 0) ? -1 : 1;
        total += a/((2*i)-1);
    }
    return total*4;
}

int main(int argc, char* argv[]){

    int n_termos = atoll(argv[1]);           // conversão para int / long long   TEMP
    int n_filhos = atoi(argv[2]);
