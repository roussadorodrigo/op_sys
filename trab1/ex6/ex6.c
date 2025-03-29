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

    if(argc < 3) return 1;                                              // validação número de argumentos

    unsigned long long n_termos = atoll(argv[1]);                       // conversão de argumento para long long
    if(n_termos < MIN_VALUE) n_termos = MIN_VALUE;                  // verificar o número de termos, minimo é 10 000 000 000
    printf("%llu\n", n_termos);

    unsigned long long n_filhos = atoi(argv[2]);                        // conversão de argumento para long long
    if(n_filhos < 1) n_filhos = 1;                                      // verificar o número de filhos, minimo é 1
    printf("%llu\n", n_filhos);

    unsigned long long n_termos_cada = n_termos/n_filhos;               // divisão de trabalho incluindo o resto
    unsigned long long resto = n_termos%n_filhos;
    printf("%llu\n", n_termos_cada);
    printf("%llu\n", resto);

    int fd[n_filhos][2];
    pid_t pids[n_filhos];

    for (int i = 0; i < n_filhos; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            return 1;
        }

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return 1;
        }

        if (pids[i] == 0) { // Processos filho
            close(fd[i][PIPE_RE]); // Fecha leitura
            unsigned long long start = i * n_termos_cada + 1;
            unsigned long long end = start + n_termos_cada - 1;
            if (i == n_filhos - 1) end += resto;

            printf("%llu\n", start);
            printf("%llu\n", end);

            long double parcial = part_leibniz(start, end);
            printf("Parte: %.60Lf\n", parcial);
            write(fd[i][PIPE_WR], &parcial, sizeof(parcial));
            close(fd[i][PIPE_WR]);
            exit(0);
        }
    }

    // Processo pai
    long double pi = 0.0;
    for (int i = 0; i < n_filhos; i++) {
        close(fd[i][PIPE_WR]); // Fecha escrita
        long double resultado_parcial;
        read(fd[i][PIPE_RE], &resultado_parcial, sizeof(resultado_parcial));
        close(fd[i][PIPE_RE]);
        pi += resultado_parcial;
    }

    for(int i = 0; i < n_filhos; i++){
        wait(NULL);
    }


    printf("Valor aproximado de pi: %.60Lf\n", pi);
    return 0;



}

