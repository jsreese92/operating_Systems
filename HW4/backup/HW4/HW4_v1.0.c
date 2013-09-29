#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  int inputPipe[2];
  pid_t inputChild;
  char inputChar;

  if(pipe(inputPipe) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  inputChild = fork();
  if (inputChild == -1) {
    perror("inputChild fork");
    exit(EXIT_FAILURE);
  }
  if (inputChild == 0){ // child process
    close(inputPipe[1]); // close unused write end
    while (read(inputPipe[0], &inputChar, 1) >0){
      printf("%c", inputChar);
    }
    printf("\n");
    close(inputPipe[0]); // close read end once done reading
    exit(EXIT_SUCCESS);
  }
  else { // inputChild's parent process
    close(inputPipe[0]); // close unused read end
    int x;
    while (x != EOF){
      x = getchar();
      write(inputPipe[1], &x, 1);
    }
    close(inputPipe[1]); // close write end once done writing
    while (wait((int *) 0) != inputChild); // wait for child
  }
}
