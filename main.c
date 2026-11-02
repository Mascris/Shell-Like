#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <threads.h>
#include <unistd.h>
#include <fcntl.h>

char *builtins[] = {"type", "echo", "exit", "pwd", "cd",NULL};


int check_builtin(char *name){
  for (int i = 0; builtins[i] != NULL; i++)
    if (strcmp(name, builtins[i])== 0)
      return 1;
  return 0;
}

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

int process(char **args, char *in_file, char *out_file, char *err_file){
  char *save = NULL;
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork Failed");
    return -1;
  }
  if (pid == 0) {
    if (in_file != NULL) {
      int fd = open(in_file, O_RDONLY);
      if (fd < 0) {
        perror("Open input");
        exit(1);
      }
      dup2(fd, STDIN_FILENO);
      close(fd);
    }
    if (out_file != NULL) {
      int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        perror("Open Output");
        exit(1);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }
    if (err_file != NULL) {
      int fd = open(err_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        perror("Open error");
        exit(1);
      }
      dup2(fd, STDERR_FILENO);
      close(fd);
    }
    execvp(args[0], args);
    fprintf(stderr, "%s: command not found\n",args[0]);
    exit(1);
  }
  int status;
    waitpid(pid,&status,0);
  return WIFEXITED(status) ? WEXITSTATUS(status) : -1;

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
    input[strcspn(input, "\n")] = '\0';


    //tokenization
    int i = 0;
    char *tokens[128];
    for (char *arg = strtok(input, " "); arg != NULL; arg = strtok(NULL, " ") ) {
      tokens[i++] =  arg;
    }
    tokens[i] = NULL;

    char *in_file =NULL;
    char *err_file =NULL;
    char *out_file =NULL;
    for(int i = 0; tokens[i] != NULL; i++) {
      if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "1>") == 0) {
        tokens[i] = NULL;
        out_file = tokens[i+1];
        i++;
      }
      else if (strcmp(tokens[i], "2>") == 0) {
        tokens[i] = NULL;
        err_file = tokens[i+1];
        i++;
      }
      else if (strcmp(tokens[i], "<") == 0) {
        tokens[i] = NULL;
        err_file = tokens[i+1];
        i++;
      }
    }

    if (tokens[0] == NULL)
      continue;

    if (strcmp(tokens[0], "exit") == 0)
      break;

    else if (strcmp(tokens[0], "echo") == 0){
      for (int i = 1; tokens[i] != NULL; i++) {
        printf("%s%s", tokens[i], tokens[i+1] ? " ":"");
      }
      printf("\n");
    }

    else if (strcmp(tokens[0], "pwd") == 0) {
      char *cwd = curent_dir();
      printf("%s\n",cwd);
      free(cwd);
    }

    else if (strcmp(tokens[0], "cd") == 0) {
      if (strcmp(tokens[1], "~") == 0 || tokens[1] == NULL) {
        if (change_dir_home() != 0) {
          fprintf(stderr, "HOME is not set\n");
        }
      }
      else{
        char *target = tokens[1];
        int save = change_dir(target);
          if (save != 0) {
            fprintf(stderr,"cd: %s: No such file or directory\n",target);
          }
      }
    }
    else {
    process(tokens, in_file, out_file, err_file);
    }
  }
  return 0;
}

