#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


char* level_to_str(LOG_LEVEL level){
    switch (level) {
        case INFO: return "INFO";
        case ERROR: return "ERROR";
        case DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void get_date_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}



FILE* logfile = NULL;

int log_init (const char *pathname){
    logfile = fopen(pathname, "a");        //cada vez que fazemos inti queremos adicionar linhas de log sem apagar nada anteriormente "logged"
    if(logfile == NULL)return -1;
    return 0;
}


int log_message(LOG_LEVEL level, const char *msg){
    char date_time[20];                
    get_date_time(date_time, sizeof(date_time));
    fprintf(logfile, "[%s] %s - %s\n", level_to_str(level), date_time, msg);
    return 0;
}

int log_close(){
    if(logfile == NULL) return -1;
    fclose(logfile);
    return 0;
}

