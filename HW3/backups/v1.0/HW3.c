#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "buffer.h"

// TODO
#define MAX_BUFF_SIZE 6969
#define LINE_TO_PRINT_SIZE 80

// since we only pass one function and argument to st_thread_create
typedef struct {
  BoundedBuffer *from; // buffer from which input will be consumed
  BoundedBuffer *to; // buffer to which output will be deposited
  semaphore *bin_sem; //binary semaphore to provide mutual exclusion
} ThreadInit;

// function prototypes
void *getInput(void *t);
void *newlineToSpace(void *t);
void *asterisksToCarat(void *t);
void *print80CharLines(void *t);

main(){
  // initialize the ST library runtime, necessary for up() and down()
  st_init();

  //BoundedBuffer into which input from stdin is stored
  BoundedBuffer theInput;
  createBuffer(&theInput, MAX_BUFF_SIZE);

  // create a binary semaphore for use with input thread
  semaphore input_sem;
  createSem(&input_sem, 1);

  ThreadInit getInputThread = {
    NULL, // no input buffer for getInput thread, comes from stdin
    &theInput, // puts data from stdin into BoundedBuffer theInput 
    &input_sem // for mutual exclusion
  };

  // create input thread
  if (st_thread_create(getInput, &getInputThread, 0, 0) == NULL){
    perror("st_thread_create for getInput failure");
    exit(1);
  }

  //BoundedBuffer into which processed data w/o newlines are stored
  BoundedBuffer newLinesProcessed;
  createBuffer(&newLinesProcessed, MAX_BUFF_SIZE);
  
  // create binary semaphore for use with newline thread
  semaphore nl_sem;
  createSem(&nl_sem, 1);

  ThreadInit nlToSpaceThread = {
    &theInput, // processes input from the BoundedBuffer theInput
    &newLinesProcessed, // puts processed data into BB newLinesProcessed
    &nl_sem // for mutual exclusion
  };

  //create newline processing thread
  if (st_thread_create(newlineToSpace, &nlToSpaceThread,0,0) == NULL){
    perror("st_thread_create for newlineToSpace failure");
    exit(1);
  }


  //BoundedBuffer into which processed data after asterisks are handled go
  BoundedBuffer caratsProcessed;
  createBuffer(&caratsProcessed, MAX_BUFF_SIZE);
  
  // create binary semaphore for use with newline thread
  semaphore carat_sem;
  createSem(&carat_sem, 1);

  ThreadInit caratThread = {
    &newLinesProcessed, // buffer from wich input is processed
    &caratsProcessed, // puts processed data into BB caratsProcessed
    &carat_sem // for mutual exclusion
  };

  //create asterisks to carat processing thread
  if (st_thread_create(asterisksToCarat, &caratThread,0,0) == NULL){
    perror("st_thread_create for newlineToSpace failure");
    exit(1);
  }

  // create binary semaphore for use with print thread
  semaphore print_sem;
  createSem(&print_sem, 1);

  ThreadInit printThread = {
    &caratsProcessed, // buffer from which input is processed
    NULL, // no output buffer, just printing to stdout
    &print_sem // for mutual exclusion
  };

  // create print thread
  if (st_thread_create(print80CharLines, &printThread,0,0) == NULL){
    perror("st_thread_create for print80CharLines failure");
    exit(1);
  }

  // exit main thread
  st_thread_exit(NULL);
}

// thread to get input from stdin
void *getInput(void *t){
  ThreadInit *init = t;

  int theCharacter;
  int count = 0;
  while(theCharacter != EOF){
    down(init->bin_sem);
    theCharacter = getchar();
    produce(init->to, &theCharacter);
    count++;
    up(init->bin_sem);
  }

  // lets next buffer know when to stop consuming
  down(init->bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->bin_sem);

  // done, exit thread
  printf("EXIT INPUT\n");
  st_thread_exit(NULL);
}

// thread to change newlines to spaces
void *newlineToSpace(void *t){
  ThreadInit *init = t;
  char theCharacter;
  char space = ' '; //used for produce() in the case it needs a space
  int toBreak = 0; //toBreak is boolean initialized to FALSE
  int printed = 0;

  while(toBreak == 0) {
    //if "from" is done inserting, and there are no elements, break
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0)){
        toBreak = 1;
      }
    }

    if (toBreak == 0){ //if toBreak is FALSE
      down(init->bin_sem);
      theCharacter = consume(init->from);
      if (theCharacter == '\n'){
        produce(init->to, &space);
      }
      else{
        produce(init->to, &theCharacter);
      }
      up(init->bin_sem);
    }
  }

  // lets next buffer know when to stop consuming
  down(init->bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->bin_sem);

  //done, exit thread
  printf("EXIT NL\n");
  st_thread_exit(NULL);
}

void *asterisksToCarat(void *t){
  ThreadInit *init = t;
  char theCharacter;
  char nextCharacter;
  char carat = '^'; //used for produce() if it needs a carat
  char null = '\0';
  int toBreak = 0;

  while(toBreak == 0) {
    //if from is done inserting and there are no elements, break
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0))
        toBreak = 1;
    }

    if (toBreak == 0){ 
      down(init->bin_sem);
      theCharacter = consume(init->from);
      if (theCharacter == '*'){
        nextCharacter = consume(init->from);
        if (nextCharacter == '*'){
          produce(init->to, &carat);
        }
        else {
          produce(init->to, &theCharacter);
          produce(init->to, &nextCharacter); 
        }
      }
      else{ 
        printf("producing: [%c]\n", theCharacter);
        produce(init->to, &theCharacter);
      }
      up(init->bin_sem);
    }
  }

  // lets next buffer know when to stop consuming
  down(init->bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->bin_sem);

  // done, exit thread
  printf("EXIT ASTERISK\n");
  st_thread_exit(NULL);
}

void *print80CharLines(void *t){
  ThreadInit *init = t;
  char theCharacter;
  char toPrint[LINE_TO_PRINT_SIZE + 1] = ""; // +1 for null-termination
  int index = 0;
  int toBreak = 0;

while(toBreak == 0) {
    //if from is done inserting and there are no elements, break
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0)){
        toBreak = 1;
      }
    }

    if (toBreak == 0){
      down(init->bin_sem);
      //deadlock on consume unless extra produce in asterisk
      theCharacter = consume(init->from);
      printf("consumed: [%c]\n", theCharacter);
      toPrint[index] = theCharacter;
      index++;
      if (index == LINE_TO_PRINT_SIZE){
        toPrint[80] = '\0'; // terminates string
        printf("%s\n", toPrint);
        toPrint[0] = '\0'; // resets string
      }
      up(init->bin_sem);
    }
  }

  // done, exit thread
  printf("EXIT PRINT\n");
  st_thread_exit(NULL);
}
