#include "shell.h"

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

    int i = 0;
    char *tokens[128] = {0};
    for (char *arg = strtok(input, " "); arg != NULL; arg = strtok(NULL, " ") ) {
      tokens[i++] =  arg;
    }
    tokens[i] = NULL;

    char *in_file =NULL;
    char *err_file =NULL;
    char *out_file =NULL;
    for(int j = 0; tokens[j] != NULL; j++) {
      if (strcmp(tokens[j], ">") == 0 || strcmp(tokens[j], "1>") == 0) {
        tokens[j] = NULL;
        out_file = tokens[j+1];
        j++;
      }
      else if (strcmp(tokens[j], "2>") == 0) {
        tokens[j] = NULL;
        err_file = tokens[j+1];
        j++;
      }
      else if (strcmp(tokens[j], "<") == 0) {
        tokens[j] = NULL;
        in_file = tokens[j+1];
        j++;
      }
    }

    if (tokens[0] == NULL)
      continue;

    int saved_out = -1;
    if(out_file != NULL && check_builtin(tokens[0])){
      saved_out = dup(1);
      int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      dup2(fd, 1);
      close(fd);
    }
  
    if (strcmp(tokens[0], "exit") == 0)
      break;

    else if (strcmp(tokens[0], "echo") == 0){
      for (int i = 1; tokens[i] != NULL; i++) {
        printf("%s%s", tokens[i], tokens[i+1] ? " " : "");
      }
      printf("\n");
    }

    else if (strcmp(tokens[0], "pwd") == 0) {
      char *cwd = curent_dir();
      printf("%s\n",cwd);
      free(cwd);
    }

    else if (strcmp(tokens[0], "cd") == 0) {
      if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0) {
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
    if (saved_out != -1) {
      fflush(stdout);
      dup2(saved_out, 1);
      close(saved_out);
    }
  }
  return 0;
}

