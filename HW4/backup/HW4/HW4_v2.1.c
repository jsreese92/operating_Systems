#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LINE_TO_PRINT_SIZE 80

// function prototypes
void inputHandler(int writePipe);
void newlineHandler(int readPipe);
void asteriskHandler(int readPipe);
void outputHandler(int readPipe);

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
      while ( wait((int *) 0) != newlineHandlingPID);
      break;
  }
}

void outputHandler(int readPipe){
  char buf;
  char toPrint[LINE_TO_PRINT_SIZE + 1] = ""; // +1 for null-termination
  int index = 0;
  while(read(readPipe, &buf, 1) > 0){
    //printf("%c", buf);
    if (buf != EOF){
      toPrint[index] = buf;
      index++;
    }
    if (index == LINE_TO_PRINT_SIZE){
      toPrint[80] = '\0'; // terminates string
      printf("%s\n", toPrint);
      toPrint[0] = '\0'; // resets string
      index = 0;
    }
  }
  close(readPipe);
  //printf("\n");
}

void asteriskHandler(int readPipe){
  int asteriskToOutputPipe[2];
  int outputHandlingPID;
  if (pipe(asteriskToOutputPipe) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  char buf;
  char buf2;
  char carat = '^';
  while (read(readPipe, &buf, 1) > 0){
    if (buf == '*'){
      if(read(readPipe, &buf2, 1) > 0){ // if not EOF
        if(buf2 == '*'){ // if both are asterisks
          write(asteriskToOutputPipe[1], &carat, 1);
      }
        else { // if only buf is an asterisk
          write(asteriskToOutputPipe[1], &buf, 1);
          write(asteriskToOutputPipe[1], &buf2, 1);
        }
      }
      else { // if EOF
        write(asteriskToOutputPipe[1], &buf, 1);
      }
    }
    else { // buf is not asterisk
      write(asteriskToOutputPipe[1], &buf, 1);
    }
  }
  close(readPipe);
  close(asteriskToOutputPipe[1]); // done writing

  switch (outputHandlingPID = fork()){
    case -1: // error
      perror("fork");
      exit(EXIT_FAILURE);
    case 0: // child
      outputHandler(asteriskToOutputPipe[0]);
      break;
    default: //parent
      while ( wait((int *) 0) != outputHandlingPID); // wait for child
      exit(EXIT_SUCCESS);
  }
}
    
void newlineHandler(int readPipe){
  int newlineToAsteriskPipe[2];
  int asteriskHandlingPID;
  if (pipe(newlineToAsteriskPipe) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  char buf;
  char space = ' ';
  while (read(readPipe, &buf, 1) > 0){
    if (buf == '\n')
      write(newlineToAsteriskPipe[1], &space, 1);
    else
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

    default: // parent
      while ( wait((int *) 0) != asteriskHandlingPID); // wait for child
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
