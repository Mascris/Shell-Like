#include "shell.h"

char *builtins[] = {"type", "echo", "exit", "pwd", "cd",NULL};

int check_builtin(char *name){
  for (int i = 0; builtins[i] != NULL; i++)
    if (strcmp(name, builtins[i])== 0)
      return 1;
  return 0;
}

char *curent_dir(void){
  return getcwd(NULL, 0);
}

int change_dir(char *name){
  if (chdir(name) == 0)
    return 0;
  else
    return 1;
}

int change_dir_home(void){
  char *home = getenv("HOME");
  if (chdir(home) == 0)
    return 0;
  else 
    return 1;
}
