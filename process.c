#include "shell.h"

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
