#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define LOG 1 // 0: hide logs, other: show logs

// logger
void logger(char* log, ...){
    if(LOG == 0){
        return;
    }
    va_list args;
    va_start(args, log);

    vprintf(log, args);
}