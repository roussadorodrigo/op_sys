
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_RE 0
#define PIPE_WR 1

int main(){
    int n;
    size_t read_status;
    size_t write_status;

    /*Abertura do Pipe*/
    int pipefd[2];
    pipe(pipefd);
    if(pipe(pipefd) == -1){
        perror("Calling pipe");
        exit(EXIT_FAILURE);
    }

    /*Criação do Processo Filho*/
    pid_t son_pid = fork();
    if(son_pid == -1){
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    //PROCESSO FILHO
    if(son_pid == 0){
        if(close(pipefd[PIPE_WR]) < 0){
            perror("Closing pipe");
        }

        while((read_status = read(pipefd[PIPE_RE], &n, sizeof(int))) > 0){ //enquanto o pipe estiver aberto
            n = n*n;
            printf("%d\n", n);
        }
        if(read_status == -1){
            perror("Read");
        }

        close(pipefd[PIPE_RE]);
        printf("Filho a terminar\n");
        exit(EXIT_SUCCESS);
    }

    //PROCESSO PAI
    close(pipefd[PIPE_RE]);
    int retscan;
    printf("Insira um número: ");
    while((retscan = scanf("%d", &n)) != EOF){
        if(retscan != 1){
            printf("Erro");
            break;
        }
        write_status = write(pipefd[PIPE_WR], &n, sizeof(int));
        if(write_status == -1){
            perror("Write");
        }
    }

    close(pipefd[PIPE_WR]);
    printf("Pai a terminar\n");
}
