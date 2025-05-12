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

//--------------//
//  TCP SERVER  //
//--------------//

int tcp_server_socket_init (int serverPort){

    int  serverSocket;
    struct sockaddr_in serv_addr;

    //CRIAÇÃO DO SOCKET
    if ( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        fatalErrorSystem("Erro ao criar socket\n");
    }
    
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(serverPort);

    //BIND
    if ( bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    fatalErrorSystem("Erro ao efectuar o bind\n");


    //LISTEN
    if (listen( serverSocket, 5) < 0 ){
        fatalErrorSystem("Erro no listen\n");
    }

    //WAIT
    for(int run = 1; run; ){

        printf ("A espera de ligação\n");
    }

    return serverSocket;
 
}

int tcp_server_socket_accept (int serverSocket){

    struct sockaddr_in client_addr;
    socklen_t dim_client = sizeof(client_addr);

    int newSock = accept(serverSocket, (struct sockaddr *)(&client_addr), &dim_client);

    if ( newSock < 0 )
        fatalErrorSystem("Erro no accept\n");
        
    printf("Ligação Estabelecida\n");
}

//--------------//
//  TCP CLIENT  //
//--------------//

int tcp_client_socket_init (const char *host, int port){

    int clientSocket;
    struct sockaddr_in serverAddr;

    //CRIAÇÃO DO SOCKET
    if ( (clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao pedir o descritor\n");
    }

    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = host;       
    serv_addr.sin_port        = htons(port);

    //CONNECT
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        fatalErrorSystem("Falha no connect\n");
    }
    
    printf("Ligação estabelicida ao servidor TCP: %s; porta:%d\n", host, port);

    return clientSocket;
  
}

//---------------//
//  UNIX SERVER  //
//---------------//

int un_server_socket_init (const char *serverEndPoint){


    int serverSocket;
    struct sockaddr_un serv_addr;

    //CRIAÇÃO DO SOCKET
    if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        fatalErrorSystem ("Erro ao criar o Socket\n");
    }

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, serverEndPoint);

    //BIND
    if (bind(serverSocket,(struct sockaddr *)&serv_addr, sizeof (serv_addr)) == -1){

        fatalErrorSystem ("Erro ao Fazer o Bind\n");

    }

    //LISTEN
    if (listen(serverSocket, 5) < 0){

        fatalErrorSystem ("Erro no Listen\n");
    }

    //WAIT
    for(int run = 1; run; ){

        printf ("A espera de ligação\n");
    }

    return serverSocket;

}

int un_server_socket_accept (int serverSocket){

    struct sockaddr_un client_addr;
    socklen_t dim_client = sizeof (client_addr);

    int newSock = accept (serverSocket, (struct sockaddr *)(&client_addr), &dim_client);

    if(newSock == -1){
        fatalErrorSystem ("Erro no accept\n");
    }

    printf ("Ligação estabelecida\n");
}


//---------------//
//  UNIX CLIENT  //
//---------------//

int un_client_socket_init (const char *serverEndPoint){

    int clientSocket;
    struct sockaddr_un serv_addr;

    //CRIAÇÃO DO SOCKET
    if ( (clientSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao criar o socket");
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
    
    printf("Ligacao Estabelecida\n");

}
