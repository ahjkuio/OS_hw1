#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  
  if(pipe(p) < 0){
    fprintf(2, "pipe error!\n");
    exit(1);
  }

  int pid = fork();
  
  if(pid > 0){ // родительский процесс
    close(p[0]);
    
    // записываем все аргументы, начиная с первого
    for(int i = 1; i < argc; i++){
      if(write(p[1], argv[i], strlen(argv[i])) < 0){
        fprintf(2, "write error!\n");
        close(p[1]);
        exit(1);
      }
      
      if(write(p[1], "\n", 1) < 0){
        fprintf(2, "write error!\n");
        close(p[1]);
        exit(1);
      }
    }
    
    close(p[1]);

    int status;
    wait(&status);
    if(status != 0){
      fprintf(2, "child process failed with status %d\n", status);
    }
    
  } else if(pid == 0){ // дочерний процесс
    close(p[1]);
    
    if(close(0) < 0){
      fprintf(2, "failed to close stdin\n");
      exit(1);
    }
    
    if(dup(p[0]) < 0){
      fprintf(2, "dup failed\n");
      exit(1);
    }
    
    close(p[0]);
    
    char *wc_argv[] = {"/wc", 0};
    if(exec("/wc", wc_argv) < 0){
      fprintf(2, "exec wc failed!\n");
      exit(1);
    }
    
  } else {
    fprintf(2, "fork error!\n");
    exit(1);
  }

  exit(0);
}

