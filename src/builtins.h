#ifndef _BBSH_SRC_BUILTINS_H
#define _BBSH_SRC_BUILTINS_H

#include <stdbool.h>
#include "bbsh_types.h"

size_t bbsh_num_builtins();
bool bbsh_cd(arg_t agrs);
bool bbsh_exit(arg_t args);
bool bbsh_pwd(arg_t args);
bool bbsh_help(arg_t args);

extern char *bbsh_builtins_str[];
extern bool (*bbsh_builtins[]) (arg_t args);

#endif