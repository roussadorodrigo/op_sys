#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define DEFAULT_PORT 8000
#define DEFAULT_HOST "localhost"


void * serveClient(){return NULL;}


int main(){
    int server_socket_tcp = tcp_server_socket_init(DEFAULT_PORT);
    int server_socket_un = un_server_socket_init(AF_UNIX);

    while(1){
        if(tcp_server_socket_accept(server_socket_tcp) == 0|| un_server_socket_accept(server_socket_un == 0)){
            pthread_t new_thread;
            pthread_create(&new_thread, NULL, serveClient, NULL);
            pthread_detach(new_thread);
        }
    }

    exit(EXIT_FAILURE);
}
