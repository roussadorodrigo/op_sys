#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_RE 0
#define PIPE_WR 1

int main(){
    /*Abertura do Pipe*/
    int pipefd[2];
    pipe(pipefd);

    char buff[MAX_BUFF + 1];

    /*Criação do Processo Filho*/
    pid_t son_pid = fork();
    if(son_pid == -1){
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    //PROCESSO FILHO
    if(son_pid == 0){
        close(pipefd[PIPE_WR]);
        while(read(pipefd[PIPE_RE], buff) > 0){ //enquanto o pipe estiver aberto
            
        }

        exit(EXIT_SUCCESS);
    }

    //PROCESSO PAI
    close(pipefd[PIPE_RE]);
    while(scanf("Insira um número: %s", buff) != EOF){
        write(pipefd[PIPE_WR], buff, sizeof(buff));
    }
}
