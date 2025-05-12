#include "logger.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024
#define LOG_PERMISSIONS 0664

static int log_fd = -1; //descritor de ficheiro (inicialmente "fechado")

int log_init(const char * pathname){
    const char * welcome_msg = "INÍCIO DO SERVER LOG:\n\n";

    log_fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, LOG_PERMISSIONS);
    if(log_fd == -1)
        return 0;

    if(write(log_fd, welcome_msg, strlen(welcome_msg)) == -1)
        return 0;

    return 1;
    
}


int log_message(LOG_LEVEL level, const char *msg){
    char buffer[MAX_BUFFER];

    struct tm * time_struct;
    time_t t = time(NULL);

    char * level_str;

    switch(level){
        case 0:
            level_str = "INFO";
            break;

        case 1:
            level_str = "ERROR";
            break;

        case 2:
            level_str = "DEBUG";
            break;
    }

    sprintf(buffer,"[%s] %s - %s\n\n",level_str, asctime(localtime(&t)), msg);
    if(write(log_fd, buffer, strlen(buffer)) == -1)
        return 0;

    return 1;
}


int log_message_width_end_point(LOG_LEVEL level, const char *msg, int sock){
    char buffer[MAX_BUFFER];

    struct sockaddr_in socket;
    socklen_t sock_lenght;

    getsockname(sock, (struct sockaddr*)&socket, &sock_lenght);

    sprintf(buffer, "%s: IP = %d, Port = %d", msg, socket.sin_addr.s_addr, socket.sin_port);
    return(log_message(level, buffer));
}


int log_close(){
    if(close(log_fd) == -1)
        return 0;

    return 1;
}
