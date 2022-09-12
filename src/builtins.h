#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdbool.h>

size_t bbsh_num_builtins();
bool bbsh_cd(char **agrs);
bool bbsh_exit(char **args);
bool bbsh_pwd(char **args);
bool bbsh_help(char **args);

extern char *bbsh_builtins_str[];
extern bool (*bbsh_builtins[]) (char **);

#endif