#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

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

struct hostent *


}
