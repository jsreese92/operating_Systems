#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "buffer.h"

//TODO
#define MAX_BUFF_SIZE 100

//since we only pass one function and argument to st_thread_create
typedef struct {
  BoundedBuffer *from;
  BoundedBuffer *to;
  semaphore *bin_sem;
} ThreadInit;

//function prototypes
void *getInput(void *b);
void *newlineToSpace(void *b);

main(){
  //initialize the ST library runtime, necessary for up() and down()
  st_init();

  BoundedBuffer theInput;
  createBuffer(&theInput, MAX_BUFF_SIZE);

  //create a binary semaphore initialized to 1
  semaphore bin_sem;
  createSem(&bin_sem, 1);

  //create input thread
  if (st_thread_create(getInput, &theInput, 0, 0) == NULL){
    perror("st_thread_create for getInput failure");
    exit(1);
  }

  
  printf("from consume: %c\n", consume(&theInput));
  printf("from consume: %c\n", consume(&theInput));
  printf("from consume: %c\n", consume(&theInput));
  printf("from consume: %c\n", consume(&theInput));
  printf("from consume: %c\n", consume(&theInput));

  

  //exit main thread
  st_thread_exit(NULL);


  /*
  char *p;
  *p = 'a';
  printf("theInput size: %d\n", *theInput.bufferSize);
  produce(&theInput, p);
  char fromConsume = consume(&theInput);
  printf("fromConsume:[%c]\n", fromConsume);

  printf("theInput.nextIn = %d\n", theInput.nextIn);
  printf("theInput.nextOut = %d\n", theInput.nextOut);
  */

}

//thread to get input from stdin
void *getInput(void *b){
  BoundedBuffer *theBuffer = b;
  int theCharacter;
  while(theCharacter != EOF){
    down(theBuffer->bin_sem);
    theCharacter = getchar();
    produce(theBuffer, &theCharacter);
    up(theBuffer->bin_sem);
  }
  //done, exit thread
  st_thread_exit(NULL);
}

void *newlineToSpace(void *b){
}

