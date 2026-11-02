#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int process(char **args, char *in_file, char *out_file, char *err_file);
char *locate_executable(char *name);
int check_builtin(char *name);

//directory Declarations
char *curent_dir(void);
int change_dir(char *name);
int change_dir_home();

#endif 
