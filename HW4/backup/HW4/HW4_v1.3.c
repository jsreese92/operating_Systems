#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// function prototypes
void inputHandler(int writePipe);
void newlineHandler(int readPipe);
void asteriskHandler(int readPipe);

main(){
  int inputToNewlinePipe[2];
  int newlineHandlingPID;
  if(pipe(inputToNewlinePipe) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  switch (newlineHandlingPID = fork()) {
    case -1: // error
      perror("fork");
      exit(EXIT_FAILURE);

    case 0: // child
      close(inputToNewlinePipe[1]); // close unused write descriptor
      newlineHandler(inputToNewlinePipe[0]);
      break;

    default: // parent
      close(inputToNewlinePipe[0]); // close unused read descriptor
      inputHandler(inputToNewlinePipe[1]);
      break;
  }
}

void asteriskHandler(int readPipe){
  char buf;
  while (read(readPipe, &buf, 1) > 0){
    printf("%c", buf);
  }
  printf("\n");
  close(readPipe);
}
    
void newlineHandler(int readPipe){
  int newlineToAsteriskPipe[2];
  int asteriskHandlingPID;
  if (pipe(newlineToAsteriskPipe) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  char buf;
  while (read(readPipe, &buf, 1) > 0){
    //printf("%c", buf);
    write(newlineToAsteriskPipe[1], &buf, 1);
  }
  close(readPipe);
  close(newlineToAsteriskPipe[1]);

  switch (asteriskHandlingPID = fork()){
    case -1: // error
      perror("fork");
      exit(EXIT_FAILURE);
    
    case 0: // child
      asteriskHandler(newlineToAsteriskPipe[0]);
      break;

    default:
      exit(EXIT_SUCCESS);

  }
}

void inputHandler(int writePipe){
  int x;
  while (x != EOF){
    x = getchar();
    write(writePipe, &x, 1);
  }
  close(writePipe);
}
