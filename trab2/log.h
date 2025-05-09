#ifndef LOG_H
#define LOG_H

typedef enum {INFO, ERROR, DEBUG} LOG_LEVEL;

int log_init (const char *pathname);
//inicia o ficheiro onde serão guardados os registos.

int log_message(LOG_LEVEL level, const char *msg);
//Crie e escreve no ficheiro um registo com o seguinte formato:
////[INFO] 2025-04-03 14:15:00 - TCP server shutting down...

int log_message_width_end_point(LOG_LEVEL level, const char *msg, int sock);
//Crie e escreve no ficheiro um registo com o seguinte formato:
////[INFO] 2025-04-03 14:06:10 - New connection established: IP=127.0.0.1, Port=53412.

int log_close ();
//Fecha o ficheiro.

#endif
