#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "./utils.c"
#include "./built-in_command.c"


// print current directory
void print_cr_dir(){
    logger("[print_cr_dir] enter\n");

    FILE* fp;
    char* cmd = "pwd";
    char buf[256];
    
    fp = popen(cmd, "r");
    if(fp == NULL) perror("[print_cr_dir] can not open command\n");

    while(fgets(buf, 256, fp)) {
        
        // 最後の改行文字をヌル文字に置き換える
        int i=0;
        while(buf[i] != '\0'){
            if(buf[i] == '\n') buf[i] = '\0';
            i++;
        }
        
        printf("\x1b[34m%s\x1b[0m", buf);
    }
    pclose(fp);
}

// read input from standard input
char* read_line(){
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    logger("[read_line] %s", line);
    return line;
}

// analyze user input
char** split_line(char* line){
    int bufsize = 64, i = 0;
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token;

    token = strtok(line, " \t\r\n\a");
    if(token == NULL) {
        logger("[split_line] no token\n");
    }
    while (token != NULL){
        tokens[i++] = token;
        logger("[split_line] %s\n", token);

        token = strtok(NULL, "\t\r\n\a");
    }
    tokens[i] = NULL;
    return tokens;
}

// execute built-in command
int exec_builtin_command(char** args){
    if(strcmp(args[0], "cd") == 0){
        return builtin_command_cd(args);
    }
    if(strcmp(args[0], "exit") == 0){
        return builtin_command_exit(args);
    }
    return 1;
}

// execute external command
int exec_external_command(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork(); // 自身のプロセスをコピー
    if (pid == 0) { // フォークに成功
        if (execvp(args[0], args) == -1) { // 実行に失敗したとき
            perror("failed to exec\n");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // フォークに失敗したとき
        perror("failed to fork\n");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

// execute
int execute(char** args) {
    if(args == NULL){
        return 1;
    }

    if(is_builtin_command(args)){
        return exec_builtin_command(args);
    }else{
        return exec_external_command(args);
    }
}


int main(){
    char* line;
    char** args;
    int status;

    do {
        print_cr_dir();
        printf("$ ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    }while (status);

    return EXIT_SUCCESS;
}