#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "builtins.h"

char *bbsh_builtins_str[] = {
    "cd",
    "exit",
    "help",
    "pwd"
};

bool (*bbsh_builtins[]) (char **) = {
    &bbsh_cd,
    &bbsh_exit,
    &bbsh_help,
    &bbsh_pwd,
};

size_t bbsh_num_builtins() {
    return sizeof(bbsh_builtins_str) / sizeof(char *);
}

static bool check_num_args(char **args, size_t limit) {
    for (size_t i = 0; args[i] != NULL; ++i) {
        if (i == limit) {
            return false;
        }
    }

    return true;
}

bool bbsh_cd(char **args) {
    if (args[1] == NULL) {
        fputs("bbsh: cd: expected argument\n", stderr);
    } else if (!check_num_args(args, 2)) {
        fputs("bbsh: cd: too many arguments\n", stderr);
    } else {
        if (chdir(args[1]) == -1) {
            perror("bbsh: cd");
        }
    }

    return true;
}

bool bbsh_exit(char **args) {
    if (!check_num_args(args, 1)) {
        fputs("bbsh: exit: warning: too many arguments\n", stdout);
    }

    return false;
}

bool bbsh_help(char **args) {
    if (!check_num_args(args, 1)) {
        fputs("bbsh: help: warning: too many arguments\n", stdout);
    }

    puts("\nBBSH (Biraj's Basic Shell)");
    puts("Type the command / program name followed by arguments and hit ENTER.");
    puts("The following shell commands are defined internally:");

    size_t num_builtins = bbsh_num_builtins();
    for (size_t i = 0;  i < num_builtins; ++i) {
        printf("  %s\n", bbsh_builtins_str[i]);
    }

    puts("\nUse the man command for information on other programs.");
    return true;
}

#define PATH_MAX 512
char cwd[PATH_MAX];

bool bbsh_pwd(char **args) {
    if (!check_num_args(args, 1)) {
        fputs("bbsh: pwd: warning: too many arguments\n", stdout);
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        puts(cwd);
    } else {
        perror("bbsh: pwd");
    }

    return true;
}