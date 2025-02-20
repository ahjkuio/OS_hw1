#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  int mode = (argc > 1) ? atoi(argv[1]) : 0; // task1 - вариант(a); "task1 1" - вариант (b)

  pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if(pid == 0){ // дочерний процесс
    sleep(10); // даем родительскому процессу время вывести свое сообщение
    fprintf(1, "Child process (pid %d) started\n", getpid());
    sleep(50);
    exit(1);
  } else { // родительский процесс
    int status;
    fprintf(1, "Parent process (pid %d) created child (pid %d)\n", getpid(), pid);

    if(mode == 1){
      fprintf(1, "Sending kill signal to child\n");
      kill(pid);
    }

    wait(&status);
    fprintf(1, "Child process (pid %d) finished with status %d\n", pid, status);
    exit(0);
  }
}

