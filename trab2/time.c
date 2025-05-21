#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define DEFAULT_ITERATIONS 100

int function(){
	return 0;
}

void * t_function(void * arg){
	return NULL;
}


int main (int argc, char *argv[])
{
 long nTimes = DEFAULT_ITERATIONS;
 struct timespec t_start;
 struct timespec t_end;
 clock_gettime(CLOCK_MONOTONIC, &t_start);
 for (unsigned long i = 0; i < nTimes; ++i) {
 
 /*Alinea a*/
 //function();
 
 
 /*Alinea b*/
 //getpid();
 //write(STDOUT_FILENO,"",0);
 
 
 /*Alinea c*/
 /*pid_t retfork = fork();
 if(retfork == 0){
 	exit(EXIT_SUCCESS);
 }
 
 waitpid(retfork, NULL, 0);*/
 
 
 /*Alinea d*/
 /*pid_t retfork = fork();
 if(retfork == 0){
 	execlp("program.c", "program.c", NULL);
 	exit(EXIT_FAILURE);
 }
 
 waitpid(retfork, NULL, 0);*/
 
 
 
/*Alinea e*/ 
/*pthread_t th;
pthread_create(&th, NULL, t_function, NULL);
pthread_join(th, NULL);
*/

}
 
 clock_gettime(CLOCK_MONOTONIC, &t_end);
 double total_time = (t_end.tv_sec - t_start.tv_sec ) + (t_end.tv_nsec - t_start.tv_nsec )/1e9;
 double average_time = (total_time/nTimes)*1e6;
 printf("Total elapsed time = %9.6lf s (average %7.2lf us)\n", total_time, average_time);
} 
