#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *builtins[] = {"type", "echo", "exit", NULL};

//TODO: Making a Loop to search inside PATH

int check_builtin(char *name){
  for (int i = 0; builtins[i] != NULL; i++)
    if (strncmp(name, builtins[i], 4)== 0)
      return 1;
  return 0;
}

//WARNING: made progress 2026-10-14 15:56
char *locate_executable(char *name){
  char buf[64];
  char *path = getenv("PATH");
  if (path == NULL) 
    return NULL;
  char *copy = strdup(path);
  char *dir = strtok(copy, ":");
  while (dir != NULL) {
    snprintf(buf, sizeof(buf),"%s/%s", dir, name);
    if (access(buf, X_OK) == 0) 
      printf("%s is %s\n",name, buf);
    else 
      printf("%s: not found\n",name);
    dir = strtok(NULL, ":");
  }
  free(copy);
}


int  main(int argc, char *argv[])
{
  char input[100];
  

  while (1) {

  printf("$ ");
  fflush(stdout);
  fgets(input, sizeof(input), stdin);
  //input[strlen(input) - 1] = '\0';
  input[strcspn(input,"\n")] = '\0';
 

    if (strcmp(input, "exit")== 0 )
      break;

    else if (strncmp(input, "type ", 5) == 0) {
       char *arg = input + 5;
       char *dir = locate_executable(arg);

      if (check_builtin(arg))
        printf("%s: is a shell builtin command\n", arg);
      else if (locate_executable(arg)!= NULL)
        {}
      else
        printf("%s: command not found!!",arg);
    }

    else if (strncmp(input, "echo ", 5)== 0)
      printf("%s\n", input + 5);

    else 
      printf("%s: command not found!\n", input);
  }
  return 0;
}
