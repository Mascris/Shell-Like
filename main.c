#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
//check in path for builtin
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
char *getexe(char *name,char *targ){
  char *path = getenv("PATH");
  char *copy = strdup(path);
  char *save = NULL;
  char exec[1024];

  if (path == NULL) 
    return NULL;

  for (char *dir = strtok(copy, ":"); dir != NULL ; dir = strtok(NULL, ":")) {
    snprintf(exec, sizeof(exec), "%s/%s", dir,name);
      if(access(exec, X_OK) == 0){
      pid_t pid = fork();

        if (pid < 0) {
          perror("fork failed");
          return NULL;
        }

        if (pid == 0) {
        int i = 0;
        char *args[100]; 
        for (char *arg = strtok(targ, " "); arg != NULL; arg = strtok(NULL, " ")) {
          args[i++] = arg;
        }
          args[i] = NULL;
        execv(exec, args);
        perror("Failed to Create Child");
        }
      int status;
      waitpid(pid,&status,0);
        save = strdup(exec);
        break;
      }
  }
  free(copy);
  return save;
}

int main(int argc, char *argv[])
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

  //spliting name from the input for execv
  char *dup = strdup(input);
    char *name = strtok(dup, " ");

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
      
    else{
    char *save = getexe(name, input);
      if (save != NULL) {
        printf("");
        free(save);
      }
      else {
        printf("%s: command not found\n",input);
      }
    }
  }
  return 0;
}
