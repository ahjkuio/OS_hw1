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
      int res = write(p[1], argv[i], strlen(argv[i]));
      if(res < 0){
        fprintf(2, "write error!\n");
        break;
      }
      
      res = write(p[1], "\n", 1);
      if(res < 0){
        fprintf(2, "write error!\n");
        break;
      }
    }
    
    close(p[1]);
    wait(0);
    
  } else if(pid == 0){ // дочерний процесс
    close(p[1]);
    
    char c;
    int res = read(p[0], &c, 1);
    while(res > 0){
      write(1, &c, 1);
      res = read(p[0], &c, 1);
    }
    
    if(res < 0) 
      fprintf(2, "read error!\n");
    
    close(p[0]);
    exit(0);
    
  } else {
    fprintf(2, "fork error!\n");
  }

  exit(0);
}
