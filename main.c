#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *builtins[] = {"type", "echo", "exit", "pwd", "cd",NULL};

//TODO: need to make it more polished and make lesser condition

//check for Builtin command that are in builtins array
int check_builtin(char *name){
  for (int i = 0; builtins[i] != NULL; i++)
    if (strcmp(name, builtins[i])== 0)
      return 1;
  return 0;
}

//NOTE: Finished 2026-10-14 19:03
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

char *curent_dir(void){
  return getcwd(NULL, 0);
}

//NOTE: Finished this for Now
//2026-10-27 01:32
char *getexe(char *name,char *targ){
  char *path = getenv("PATH");
  char *save = NULL;
  char exec[1024];

  if (path == NULL) 
    return NULL;

  char *copy = strdup(path);
  for (char *dir = strtok(copy, ":"); dir != NULL ; dir = strtok(NULL, ":")) {
    snprintf(exec, sizeof(exec), "%s/%s", dir,name);
      if(access(exec, X_OK) == 0){
      pid_t pid = fork();

        if (pid < 0) {
          perror("fork failed");
          return NULL;
        }

        if (pid == 0) {
        //child process
        int i = 0;
        char *args[100]; 
        for (char *arg = strtok(targ, " "); arg != NULL; arg = strtok(NULL, " ")) {
          args[i++] = arg;
        }
          args[i] = NULL;
        execv(exec, args);
        perror("Failed to Create Child");
        exit(1);
        }
      //parents process
      int status;
      waitpid(pid,&status,0);
        save = strdup(exec);
        break;
      }
  }
  free(copy);
  return save;
}

int change_dir(char *name){
  if (chdir(name) == 0)
    return 0;
  else
    return 1;
}

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL);
  char input[1024];
  
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

    else if (strcmp(input, "pwd") == 0){
      char *save = curent_dir();
      printf("%s\n",save);
      free(save);
    }

    else if (strncmp(input, "cd ", 3) == 0) {
      char *target = input + 3;
      int save = change_dir(target);
      if (save != 0) {
        printf("cd: %s: No such file or directory\n",target);
      }
    }
      
    else{
    char *save = getexe(name, input);
      if (save != NULL) {
        free(save);
      }
      else {
        printf("%s: command not found\n",input);
      }
    }
  }
  return 0;
}
