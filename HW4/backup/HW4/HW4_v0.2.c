#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// function prototypes
void inputChildProcess(int theInputPipe[2], char theInputChar);
void newlineChildProcess(int theNewlinePipe[2], char theNewlineChar);

main(){
  int inputPipe[2];
  pid_t inputChildPID;
  char inputChar;

  if(pipe(inputPipe) == -1){
    perror("inputPipe");
    exit(EXIT_FAILURE);
  }

  inputChildPID = fork();
  if (inputChildPID == -1) {
    perror("inputChildPID fork");
    exit(EXIT_FAILURE);
  }
  if (inputChildPID == 0){ // child process
    inputChildProcess(inputPipe, inputChar);
  }
  else { // inputChildPID's parent process
    printf("inputChild parent\n");
    close(inputPipe[0]); // close unused read end
    printf("input read closed, not used\n");
    int x;
    while (x != EOF){
      x = getchar();
      write(inputPipe[1], &x, 1);
    }
    close(inputPipe[1]); // close write end once done writing
    printf("input write closed, done writing\n");
    while (wait((int *) 0) != inputChildPID); // wait for child
    printf("exiting parent of inputChild\n");
    exit(EXIT_SUCCESS);
  }
}

void inputChildProcess(int theInputPipe[2], char theInputChar){
  printf("inputChildProcess\n");
  int newlinePipe[2];
  pid_t newlineChildPID;
  char newlineChar;

  if (pipe(newlinePipe) == -1){
    perror("newlinePipe");
    exit(EXIT_FAILURE);
  }

  newlineChildPID = fork();
  if (newlineChildPID == -1){
    perror("newlineChildPID fork");
    exit(EXIT_FAILURE);
  }
  if (newlineChildPID == 0){ // newlineChild process
    newlineChildProcess(newlinePipe, newlineChar);
  }
  else { // newlineChild parent: inputChildProcess
    printf("newlineChild parent\n");
    close(theInputPipe[1]); // close unused write end
    printf("input write closed, not used\n");
    close(newlinePipe[0]); // close unused read end
    printf("newline read closed, not used\n");
    printf("from newlineChild parent\n");
    while (read(theInputPipe[0], &theInputChar, 1) >0){
      printf("from nl parent: [%c]\n", theInputChar);
      write(newlinePipe[1], &newlineChar, 1);
    }
    close(theInputPipe[0]); // close read end once done reading
    printf("input read closed, done reading\n");
    close(newlinePipe[1]); // close write end once done writing
    printf("newline write closed, done writing\n");
    while (wait((int *) 0) != newlineChildPID); // wait for child
    printf("exiting inputChild process\n");
    exit(EXIT_SUCCESS);
  }
}

void newlineChildProcess(int theNewlinePipe[2], char theNewlineChar){
  printf("newlineChildProcess\n");
  close(theNewlinePipe[1]); // close unused write end
  printf("newline write closed, not used\n");
  while (read(theNewlinePipe[0], &theNewlineChar, 1) > 0){
    printf("%c", theNewlineChar);
  }
  close(theNewlinePipe[0]); // close read end once done reading
  printf("newline read closed, done reading\n");
  printf("exiting newlineChildProcess\n");
  exit(EXIT_SUCCESS);
}
