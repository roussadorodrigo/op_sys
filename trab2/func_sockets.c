#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "erroraux.h"

//NADA TESTADO

int tcp_server_socket_init (int serverPort){

    int  serverSocket;
    if ( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        fatalErrorSystem("Erro ao criar socket");
    }
    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(serverPort);

    //BIND
    if ( bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    fatalErrorSystem("Erro ao efectuar o bind");


    //LISTEN
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

    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = host;       
    serv_addr.sin_port        = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        fatalErrorSystem("Falha no connect");
    }
    
    printf("Ligação estabelicida ao servidor TCP: %s; porta:%d\n", host, port);

    return clientSocket;
  
}

int un_server_socket_init (const char *serverEndPoint){


    int serverSocket;
    //Criação do socket
    if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){

        fatalErrorSystem ("Erro ao Criar o Socket");

    }

    struct sockaddr_un serv_addr;

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, serverEndPoint);

    //BIND
    if (bind(serverSocket,(struct sockaddr *)&serv_addr, sizeof (serv_addr)) == -1){

        fatalErrorSystem ("Erro ao Fazer o bind");

    }

    //LISTEN
    if (listen(serverSocket, 5) < 0){

        fatalErrorSystem ("Erro no listen");
    }

    return serverEndPoint;
    
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
    strcpy(serv_addr.sun_path, serverEndPoint);
    
    printf("O cliente vai ligar-se ao servidor no socket UNIX\n");


    //CONNECT
    if ( connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
        fatalErrorSystem("Falha no connect");
    }
    
    //ex do stor
    printf("Ligacao estabelecida...\n");


}
