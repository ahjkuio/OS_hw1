#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFFER_SIZE 1024  // Размер буфера для чтения блоками

int
main(int argc, char *argv[])
{
  int pipefd[2];
  int pid;
  int ret;
  
  if(pipe(pipefd) < 0){
    fprintf(2, "task2: ошибка создания pipe\n");
    exit(1);
  }

  pid = fork();
  
  if(pid < 0){
    fprintf(2, "task2: ошибка fork\n");
    exit(1);
  }

  if(pid == 0){
    char buffer[BUFFER_SIZE];
    int n;

    if (close(pipefd[1]) < 0) {
      fprintf(2, "task2: ошибка закрытия дескриптора записи pipe в дочернем процессе\n");
      exit(1);
    }

    while ((n = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
      if (write(1, buffer, n) != n) {
        fprintf(2, "task2: ошибка записи в stdout\n");
        exit(1);
      }
    }

    if (n < 0) {
      fprintf(2, "task2: ошибка чтения из pipe\n");
      exit(1);
    }

    if (close(pipefd[0]) < 0) {
      fprintf(2, "task2: ошибка закрытия дескриптора чтения pipe в дочернем процессе\n");
      exit(1);
    }

    exit(0);
  } else {
    if (close(pipefd[0]) < 0) {
      fprintf(2, "task2: ошибка закрытия дескриптора чтения pipe в родительском процессе\n");
      exit(1);
    }

    for(int i = 1; i < argc; i++){
      if (write(pipefd[1], argv[i], strlen(argv[i])) != strlen(argv[i])) {
        fprintf(2, "task2: ошибка записи в pipe\n");
        exit(1);
      }
      if (write(pipefd[1], "\n", 1) != 1) {
        fprintf(2, "task2: ошибка записи символа перевода строки в pipe\n");
        exit(1);
      }
    }

    if (close(pipefd[1]) < 0) {
      fprintf(2, "task2: ошибка закрытия дескриптора записи pipe в родительском процессе\n");
      exit(1);
    }

    if (wait(&ret) != pid) {
      fprintf(2, "task2: дочерний процесс не завершился корректно\n");
      exit(1);
    }

    exit(0);
  }
}
