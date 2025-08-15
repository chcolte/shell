#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

// check built-in command or not
int is_builtin_command(char** args){
    if(args == NULL){
        return 0;
    }
    
    char builtin_command[][6] = {"cd", "export", "exit"};
    int size_of_builtin_command = 3;

    for(int i=0; i<size_of_builtin_command; i++){
        if(strcmp(args[0], builtin_command[i]) == 0){
            logger("[is_builtin_command] %s is built-in command\n", args[0]);
            return 1;
        }
    }
    return 0;
}

int builtin_command_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("[builtin_command_cd] failed to change directory.\n");
        }
    }
    return 1;
}

int builtin_command_exit(char **args) {
    return 0;
}