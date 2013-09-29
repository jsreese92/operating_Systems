#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  int inputPipe[2];
  pid_t inputChild;
  char buf;
  
  if (pipe(inputPipe) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  inputChild = fork();
  if (inputChild == -1){
    perror("inputChild fork");
    exit(EXIT_FAILURE);
  }

  // takes input and changes newlines to spaces
  // creates new process asterisks pipe
  if (inputChild == 0) {
    int newlinePipe[2];
    pid_t newlineChild;
    char newlineBuf;

    if (pipe(newlinePipe) == -1){
      perror("newlinePipe");
      exit(EXIT_FAILURE);
    }

    newlineChild = fork();
    if (newlineChild == -1){
      perror("newlineChild fork");
      exit(EXIT_FAILURE);
    }

    // processes asterisks from newlinePipe
    // creates asterisks pipe 
    if (newlineChild == 0){
      char toPrint;
      close(newlinePipe[1]);
      while (read(newlinePipe[0], &toPrint, 1) > 0){
        printf("%c", toPrint);
      }
      exit(EXIT_SUCCESS);
    }
    // parent of newlineChild: inputChild
    else {
      char space = ' ';
      close(inputPipe[1]);
      close(newlinePipe[0]);
      while (read(inputPipe[0], &buf, 1) > 0){
        //printf("buf: [%c]\n", buf);
        if (buf == '\n')
          write(newlinePipe[1], &space, 1);
        else
          write(newlinePipe[1], &buf, 1);
      }
      close(newlinePipe[1]);

      close(inputPipe[0]);
      _exit(EXIT_SUCCESS);
    }
  } 
  // parent of getInput process
  else {
    close(inputPipe[0]);
    int x;
    while (x != EOF){
      x = getchar();
      write(inputPipe[1], &x, 1);
    }
//    write(inputPipe[1], "hello!\n", strlen("hello!\n"));
    close(inputPipe[1]);
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}





