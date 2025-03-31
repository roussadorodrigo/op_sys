#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_RE 0
#define PIPE_WR 1

#define SIZE_BUFF 32

int main(int argc, char * argv[]){

    int word_count = -1;

    if(argc <= 1){
        printf("Ficheiro não encontrado!\n");
        exit(EXIT_FAILURE);
    }

    char buff[SIZE_BUFF];

    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("Calling pipe");
        exit(EXIT_FAILURE);
    }

    int pid_filho = fork();
    if(pid_filho < 0){
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    //FILHO
    if(pid_filho == 0){
        close(pipefd[PIPE_RE]);
        dup2(pipefd[PIPE_WR], STDOUT_FILENO);
        if(execlp("/bin/wc", "wc", "-w", argv[1], NULL)){
            perror("Exec");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    //PAI
    close(pipefd[PIPE_WR]);
    waitpid(pid_filho, NULL, 0);
    int read_count = read(pipefd[PIPE_RE], buff, sizeof(buff));
    buff[read_count] = '\0';
    sscanf(buff, "%d", &word_count);
    printf("Word Count: %d\n", word_count);
    close(pipefd[PIPE_RE]);
    exit(EXIT_SUCCESS);

}
