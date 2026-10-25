#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *builtins[] = {"type", "echo", "exit", NULL};

//TODO: Adding a exec and argv using "fork and exec"
//TODO: need to make it more polished and make lesser condition

//check for Builtin command that are in builtins array
int check_builtin(char *name){
  for (int i = 0; builtins[i] != NULL; i++)
    if (strncmp(name, builtins[i], 4)== 0)
      return 1;
  return 0;
}

//NOTE: Finished 2026-10-14 19:03 Old Version!!
char *locate_executable(char *name){
  char buf[4096];
  char *path = getenv("PATH");
  char *save = NULL;

  if (path == NULL) 
    return NULL;
  char *copy = strdup(path);
  char *dir = strtok(copy, ":");
  while (dir != NULL) {
    snprintf(buf, sizeof(buf),"%s/%s", dir, name);
    if (access(buf, X_OK) == 0){
      save = strdup(buf);
    break;
    }
    dir = strtok(NULL, ":");
    save = NULL;
  }
  free(copy);
  return save;
}

//NOTE: Working on this Now
//get executable and execute and pass an arguments
char *getexe(char *name,const char *argv[]){
  char *path = getenv("PATH");
  char *copy = strdup(path);
  char *save = NULL;
  char exec[1024];
  pid_t pid = fork();

  if (path == NULL) 
    return NULL;
  for (char *dir = strtok(copy, ":"); dir != NULL ; dir = strtok(NULL, ":")) {
    snprintf(exec, sizeof(exec), "%s/%s", dir,name);
      if(access(exec, X_OK) == 0){
        if (pid < 0) {
          perror("fork failed");
          return NULL;
        }
        if (pid == 0) {
        int execv(exec, agrv);
        }
      }
  }
  free(copy);
  return save;
}

int  main(int argc, char *argv[])
{
  setbuf(stdout, NULL);
  char input[100];
  
  while (1) {

  printf("$ ");
  fflush(stdout);
    if (fgets(input, sizeof(input), stdin) == NULL)
      break;
  //input[strlen(input) - 1] = '\0';
  input[strcspn(input,"\n")] = '\0';

    if (strcmp(input, "exit")== 0 )
      break;

    else if (strncmp(input, "type ", 5) == 0) {
       char *arg = input + 5;

      if (check_builtin(arg))
        printf("%s is a shell builtin\n", arg);

      else if (locate_executable(arg)!= NULL){
        char *save=locate_executable(arg);

        if (save != NULL) {
        printf("%s is %s\n",arg, save);
        free(save);
        save = NULL;
        }

        else
          printf("%s: not found\n",arg);
    }

      else
        printf("%s: not found\n",arg);
    }

    else if (strncmp(input, "echo ", 5)== 0)
      printf("%s\n", input + 5);

    else 
      printf("%s: not found\n", input);
  }
  return 0;
}
