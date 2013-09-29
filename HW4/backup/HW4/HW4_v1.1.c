#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// function prototypes
void inputChildProcess(int theInputPipe[2], char theInputChar);

main(){
  int inputPipe[2];
  pid_t inputChildPID;
  char inputChar;

  if(pipe(inputPipe) == -1){
    perror("pipe");
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
    close(inputPipe[0]); // close unused read end
    int x;
    while (x != EOF){
      x = getchar();
      write(inputPipe[1], &x, 1);
    }
    close(inputPipe[1]); // close write end once done writing
    while (wait((int *) 0) != inputChildPID); // wait for child
  }
}

void inputChildProcess(int theInputPipe[2], char theInputChar){
  close(theInputPipe[1]); // close unused write end
  while (read(theInputPipe[0], &theInputChar, 1) >0){
    printf("%c", theInputChar);
  }
  printf("\n");
  close(theInputPipe[0]); // close read end once done reading
  exit(EXIT_SUCCESS);
}

