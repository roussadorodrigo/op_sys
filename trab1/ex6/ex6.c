#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#define PIPE_RE 0
#define PIPE_WR 1

#define MIN_VALUE 10000000000                                           // valor minimo de termos

long double leibniz(unsigned long long k,unsigned long long n){         //funcao serie de leibniz 
    long double total = 0;
    long double a = 0;
    for(   ; k <= n; k++){
        a = (k % 2 == 0) ? -1 : 1;
        total += a/((2*k)-1);
    }
    return total*4;
}

int main(int argc, char* argv[]){

    struct timeval t1,t2;
    gettimeofday(&t1, NULL);

    if(argc < 3){                                                       // validação número de argumentos
        printf("Não inseriu corretamente os argumentos!\n");
        exit(EXIT_FAILURE);
    }

    unsigned long long n_termos = atoll(argv[1]);                       // conversão de argumento para long long
    if(n_termos < MIN_VALUE){ 
        n_termos = MIN_VALUE;                                           // verificar o número de termos, minimo/default é 10 000 000 000
        printf("Número de termos alterado para o minimo pedido!\n");
    }
    unsigned long long n_filhos = atoi(argv[2]);                        // conversão de argumento para int
    if(n_filhos < 1){
        n_filhos = 1;                                                   // verificar o número de filhos, default é 1
        printf("Número de processos alterado para 1!\n");
    }
    

    unsigned long long n_termos_cada = n_termos/n_filhos;               // divisão de trabalho incluindo o resto
    unsigned long long resto = n_termos%n_filhos;
    

    int fd[n_filhos][2];                                                // descritor de ficheiros de cada processo
    pid_t pids[n_filhos];                                               // array onde guarda-se os pids dos processos filho

    for(int i = 0; i < n_filhos; i++){                                  // criação de pipe para cada processo filho
        if(pipe(fd[i]) == -1){
            perror("Calling pipe");
            exit(EXIT_FAILURE);
        }

        pids[i] = fork();                                               // criação de cada processo filho
        if(pids[i] < 0){
            perror("Calling fork");
            exit(EXIT_FAILURE);
        }
        // Processos filho
        if(pids[i] == 0){
            close(fd[i][PIPE_RE]);                                      // fecho das leituras do pipes

            unsigned long long start = i * n_termos_cada + 1;           // atribuição de "trabalho" para cada filho
            unsigned long long end = start + n_termos_cada - 1;
            if(i == n_filhos - 1) end += resto;                         // o ultimo processo filho fica também com o resto

            long double parcial = leibniz(start, end);                  // calculo da sua parte do "trabalho"   

            write(fd[i][PIPE_WR], &parcial, sizeof(parcial));           // escreve no pipe
            close(fd[i][PIPE_WR]);                                      // fecha o pipe
            exit(EXIT_SUCCESS);
        }
    }

    // Processo pai
    long double pi = 0.0;
    for(int i = 0; i < n_filhos; i++){
        close(fd[i][PIPE_WR]);                                          // fecho da escrita dos pipes
        long double resultado_parcial;
        read(fd[i][PIPE_RE], &resultado_parcial, sizeof(resultado_parcial));        // le do pipe
        close(fd[i][PIPE_RE]);                                                      // fecha o pipe
        pi += resultado_parcial;                                                    // incrementa ao somatorio total
    }

    for(int i = 0; i < n_filhos; i++){
        waitpid(pids[i], NULL, 0);
    }

    printf("Valor aproximado de pi: %.15Lf\n", pi);



    gettimeofday(&t2, NULL);
    long elapsed = ((long)t2.tv_sec - t1.tv_sec) * 1000000L + (t2.tv_usec - t1.tv_usec);
    printf ("Tempo de execução = %6li us\n", elapsed);
    return 0;
}


