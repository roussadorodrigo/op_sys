typedef enum {INFO, ERROR, DEBUG} LOG_LEVEL;

/*
FUNÇÃO: log_init
PARÂMETROS:
    pathname -> caminho do ficheiro de logging
RETORNO: descritor de ficheiro se sucesso, -1 se erro
*/
int log_init(const char * pathname);


/*
FUNÇÃO: log_message
PARÂMETROS:
    level -> tipo de log (enum)
    msg -> mensagem a escrever no log
RETORNO: 0 se sucesso, 1 se erro
*/
int log_message(LOG_LEVEL level, const char *msg);


/*
FUNÇÃO: log_message_width_end_point
PARÂMETROS:
    level -> tipo de log (enum)
    msg -> mensagem a escrever no log
    sock -> 
RETORNO: 0 se sucesso, 1 se erro
*/
int log_message_width_end_point(LOG_LEVEL level, const char *msg, int sock);


/*
FUNÇÃO: log_close
PARÂMETROS:
RETORNO: 0 se sucesso, 1 se erro
*/
int log_close ();
