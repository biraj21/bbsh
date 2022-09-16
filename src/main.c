#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include "builtins.h"
#include "bbsh_types.h"
#include "commons.h"

void bbsh_loop(void);
void motd();

int main(void) {
    // load config files, if any
    motd();

    // run the command loop
    bbsh_loop();

    // perform any cleanup

    return EXIT_SUCCESS;
}

#define BBSH_MOTD_DIR ""
void motd(){
    FILE* file=fopen(BBSH_MOTD_DIR"motd","r");
    if(!file){
        fclose(file);
        return;
    }
    char msg[512];
    fread(msg,512,512,file);
    fclose(file);
    printf(msg,BBSH_VERSION_STR);
}

#define BBSH_RL_BUFSIZE 1024L

char *bbsh_read_line(void) {
    size_t bufsize = BBSH_RL_BUFSIZE;
    char *buffer = malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        fputs("bbsh: allocation error\n", stderr);
        exit(EXIT_FAILURE);
    }

    size_t i = 0;
    char c;
    while (true) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[i] = '\0';
            return buffer;
        } else {
            buffer[i] = c;
        }

        ++i;

        if (i == bufsize) {
            bufsize += BBSH_RL_BUFSIZE;
            char *re_buffer = realloc(buffer, bufsize * sizeof(char));
            if (re_buffer == NULL) {
                free(buffer);
                fputs("bbsh: allocation error\n", stderr);
                exit(EXIT_FAILURE);
            }

            buffer = re_buffer;
        }
    }
}

#define BBSH_TOK_BUFSIZE 64
#define BBSH_TOK_DELIM " \t"

arg_t bbsh_split_line(char *line) {
    size_t bufsize = BBSH_TOK_BUFSIZE;
    arg_t tokens = malloc(bufsize * sizeof(void *));
    if (tokens == NULL) {
        fputs("bbsh: allocation error", stderr);
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, BBSH_TOK_DELIM);
    size_t i = 0;
    while (token != NULL) {
        tokens[i] = token;
        ++i;

        if (i == bufsize) {
            bufsize += BBSH_TOK_BUFSIZE;
            arg_t re_tokens = realloc(tokens, bufsize * sizeof(void *));
            if (re_tokens == NULL) {
                free(tokens);
                fputs("bbsh: allocation error\n", stderr);
                exit(EXIT_FAILURE);
            }

            tokens = re_tokens;
        }

        token = strtok(NULL, BBSH_TOK_DELIM);
    }

    tokens[i] = NULL;
    return tokens;
}

bool bbsh_launch(arg_t args) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("bbsh");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("bbsh");
        }

        exit(EXIT_FAILURE);
    } else {
        int status;

        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return true;
}

bool bbsh_execute(arg_t args) {
    if (args[0] == NULL) {
        return true;
    }

    size_t num_builtins = bbsh_num_builtins();
    for (size_t i = 0; i < num_builtins; ++i) {
        if (strcmp(args[0], bbsh_builtins_str[i]) == 0) {
            return (*bbsh_builtins[i])(args);
        }
    }

    return bbsh_launch(args);
}

#define BBSH_PATH_MAX 512

void bbsh_loop(void) {
    char cwd[BBSH_PATH_MAX];
    bool status = false;
    char hostname[HOST_NAME_MAX + 1];
    char username[LOGIN_NAME_MAX + 1];
    getlogin_r(username,LOGIN_NAME_MAX +1);
    gethostname(hostname, HOST_NAME_MAX + 1);

    do {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("┌─[%s@%s]-[%s]\n",username, hostname,cwd);
            printf("└──╼$ ");
        } else {
            perror("bbsh: getcwd() error");
            exit(EXIT_FAILURE);
        }

        char *line = bbsh_read_line();
        arg_t args = bbsh_split_line(line);
        status = bbsh_execute(args);

        free(line);
        free(args);
    } while (status);
}