#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  int getInputPipe[2];
  pid_t inputChild;

  char inputBuf;

  if (pipe(getInputPipe) == -1){
    perror("getInputPipe");
    exit(EXIT_FAILURE);
  }

  inputChild = fork();
  if (inputChild == -1){
    perror("inputChild fork");
    exit(EXIT_FAILURE);
  }

  // child reads from input pipe, creates newline pipe and writes to it,
  // then forks off for the newline processing 
  if (inputChild == 0) { 
    int processNewlinePipe[2];
    pid_t newlineChild;
    char newlineBuf;

    if (pipe(processNewlinePipe) == -1){
      perror("processNewlinePipe");
      exit(EXIT_FAILURE);
    }

    newlineChild = fork();
    if (newlineChild == -1){
      perror("newlineChild fork");
      exit(EXIT_FAILURE);
    }

    //DO ASTERISK PROCESSING STUFF
    if (newlineChild == 0){

      /*
      int processAsterisksPipe[2];
      pid_t asteriskChild;
      char asteriskBuf;

      printf("from newLine fork\n");
      while (read(processNewlinePipe[0], &newlineBuf, 1) > 0){
        write(STDOUT_FILENO, &asteriskBuf, 1);
      */

      exit(EXIT_SUCCESS);
    }
    else { // newline parent process

      close(getInputPipe[1]); // close unused write end
//      close(processNewlinePipe[0]); // close unused read end

      char toWrite;
      write(processNewlinePipe[1], "hello\n", strlen("hello\n"));
      printf("about to read\n");
      while(read(processNewlinePipe[0], &toWrite, 1) > 0){
//        printf("toWrite: [%c]\n", toWrite);
        write(STDOUT_FILENO, &toWrite, 1);
        printf("in read loop\n");
      }
      printf("done reading\n");


      /*
      while (read(getInputPipe[0], &toWrite, 1) > 0){
        printf("toWrite: [%c]\n", toWrite);
        write(processNewlinePipe[1], &toWrite, 1);
      }
      */
      close(getInputPipe[0]);
      close(processNewlinePipe[1]); 

      exit(EXIT_SUCCESS);
    }
    
    exit(EXIT_SUCCESS);
  }
  else { // get input parent process
    close(getInputPipe[0]); // close unused read end
    int x;
    /*
    while (x != EOF){
      x = getchar();
      write(getInputPipe[1], &x, 1);
    }
    */
    close(getInputPipe[1]);
    wait(NULL); // wait for child
    exit(EXIT_SUCCESS);
  }
}
