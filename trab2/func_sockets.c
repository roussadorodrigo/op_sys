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

//NADA TESTADO

int tcp_server_socket_init (int serverPort){

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


    // Activar socket com fila de espera de dimensao 5
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

    int clientSocket;
    struct sockaddr_in serverAddr;

    if ( (clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao pedir o descritor");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, host, &serverAddr.sin_addr) <= 0) { //converter os IP's para binário
        fatalErrorSystem("Erro, endereço IP invalido");
    }
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        fatalErrorSystem("Falha no connect");
    }
    
    printf("Ligação estabelecida ao servidor TCP: %s; porta:%d\n", host, port);

    return clientSocket;
  
}

int un_server_socket_init (const char *serverEndPoint){

    //FALTA

}

int un_server_socket_accept (int serverSocket){

    //FALTA

}

int un_client_socket_init (const char *serverEndPoint){

    int clientSocket;
    struct sockaddr_un serv_addr;

    if ( (clientSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao pedir o descritor");
    }

    
    struct sockaddr_un serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family      = AF_UNIX;
    strcpy(serv_addr.sun_path, serverName);
    
    printf("O cliente vai ligar-se ao servidor no socket UNIX\n");


    // Ligar-se ao servidor
    if ( connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
        fatalErrorSystem("Falha no connect");
    }
    
    printf("Ligacao estabelecida...\n");
}
