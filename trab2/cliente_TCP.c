#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>

#include "erroraux.h"
#include "func_sockets.h"

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 8000


int main (int argc, char* argv[]){

    char *serverName = DEFAULT_HOST;
    unsigned int serverPort = DEFAULT_PORT;

    if (argc == 3){

        serverName = argv[1];
        serverPort = atoi(argv[2]);

    }
    
    else if(argc != 1){

        fatalErrorSystem ("Argumentos Inválidos!\n");
    }

//Determinar o endereço IP 

    struct hostent *pHost;
    in_addr_t serverAddress;

    if((pHost = gethostbyname(serverName)) != NULL){

        memcpy(&serverAddress, pHost->h_addr_list[0], pHost->h_length);
    }
    else if ((serverAddress = inet_addr(serverName)) == -1){

        fatalErrorSystem ("Não foi possível determinar o endereço IP");
    }

    if((serverPort < 1) || (serverPort > 65535)){

        fatalErrorSystem ("O porto deve estar entre 1 e 65535!\n");
    }

    int tcp_client_socket_init (serverAddress, serverPort);    

}
