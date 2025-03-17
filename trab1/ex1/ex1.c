#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define NUMBER_OF_CHILDS 5
#define N_PRINTS 5
#define SLEEP_TIME 1

int main(){

	for(int i = 0; i < NUMBER_OF_CHILDS; i++){
        pid_t child_pid = fork();
        if(child_pid == -1){
            perror("Calling fork\n");
            exit(EXIT_FAILURE);
        }

        //PROCESSO FILHO
        if(child_pid == 0){
            int counter = 0;
            while(counter < N_PRINTS){
                printf("PID do pai: %d\n", getppid());
                printf("PID: %d\n", getpid());
                counter++;
                sleep(SLEEP_TIME);
            }
            exit(EXIT_SUCCESS);
        }

    }

    int status;
    int counter = 0;
    while(counter < NUMBER_OF_CHILDS){
        pid_t finished_pid = wait(&status);
        counter++;
        printf("PID: %d chegou ao fim!\n", finished_pid);
    }
}
