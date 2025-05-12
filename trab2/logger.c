#include "logger.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024

int log_init(const char * pathname){
    char * welcome_msg = "INÍCIO DO SERVER LOG:\n\n";
    int log_fd = open(pathname, O_RDWR);
    dup2(log_fd, STDOUT_FILENO);

    printf("%s\n\n", welcome_msg);
    return log_fd;
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
    printf("%s", buffer);
}


int log_message_width_end_point(LOG_LEVEL level, const char *msg, int sock){
    char buffer[MAX_BUFFER];

    struct sockaddr_in socket;
    socklen_t sock_lenght;

    getsockname(sock, (struct sockaddr*)&socket, &sock_lenght);

    sprintf(buffer, "%s: IP = %d, Port = %d", msg, socket.sin_addr.s_addr, socket.sin_port);
    log_message(level, buffer);
}


int log_close(){
    
}
