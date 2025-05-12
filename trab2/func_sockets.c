#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "erroraux.h"


int tcp_server_socket_init (int serverPort){

    /// Não testado
    int  serverSocket;
    if ( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        fatalErrorSystem("Erro ao pedir o descritor");
    
    // Registar endereco local de modo a que os clientes nos possam contactar 
    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(serverPort);

    if ( bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    fatalErrorSystem("Erro ao efectuar o bind");


    // Ativar socket com fila de espera de dimensao 5
    if (listen( serverSocket, 5) < 0 )
        fatalErrorSystem("Erro no listen");
    
    return serverSocket;
 
}

int tcp_server_socket_accept (int serverSocket){

    struct sockaddr_in client_addr;
    socklen_t dim_client = sizeof(client_addr);
    int newSock = accept(serverSocket, (struct sockaddr *)(&client_addr), &dim_client);
    if ( newSock < 0 )
        fatalErrorSystem("Erro ao efectuar o accept");
        
    printf("Ligação Estabelecida\n");

    return newSock;

    
  
}

int tcp_client_socket_init (const char *host, int port){

  
}

int un_server_socket_init (const char *serverEndPoint){

  
}

int un_server_socket_accept (int serverSocket){

  
}

int un_client_socket_init (const char *serverEndPoint){

  
}
