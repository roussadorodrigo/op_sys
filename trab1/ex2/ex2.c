#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    int status1, status2;

    pid_t retfork1 = fork();
    if(retfork1 == -1){
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    //PROCESSO FILHO 1
    if(retfork1 == 0){
        execlp("/bin/date", "date", NULL);
        perror("Date");
        exit(EXIT_FAILURE); //erro ao executar o execlp
    }


    int retfork2 = fork();
    if(retfork2 == -1){
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    //PROCESSO FILHO 2
    if(retfork2 == 0){
        execlp("/bin/ping", "ping", "-c", "4", "www.google.com", NULL);
        perror("Ping");
        exit(EXIT_FAILURE);
    }

    waitpid(retfork1, &status1, 0);
    if(WIFEXITED(status1)){
        printf("PID %d terminou com valor: %d\n", retfork1, WEXITSTATUS(status1));
    }

    waitpid(retfork2, &status2, 0);
    if(WIFEXITED(status2)){
        printf("PID %d terminou com valor: %d\n", retfork2, WEXITSTATUS(status2));
    }

}
