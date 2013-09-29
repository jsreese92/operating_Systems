#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "buffer.h"

#define MAX_BUFF_SIZE 10000
#define LINE_TO_PRINT_SIZE 80

// since we only pass one function and argument to st_thread_create
typedef struct {
  BoundedBuffer *from; // buffer from which input will be consumed
  BoundedBuffer *to; // buffer to which output will be deposited
  semaphore *from_bin_sem; //binary semaphore for "from" BB
  semaphore *to_bin_sem; //binary semaphore for "to" BB
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

  // create a binary semaphore for use with input buffer
  semaphore input_sem;
  createSem(&input_sem, 1);

  ThreadInit getInputThread = {
    NULL, // no input buffer for getInput thread, comes from stdin
    &theInput, // puts data from stdin into BoundedBuffer theInput 
    NULL, //no "from" semaphore, getting data from stdin
    &input_sem //mutual exclusion for input buffer
  };

  // create input thread
  if (st_thread_create(getInput, &getInputThread, 0, 0) == NULL){
    perror("st_thread_create for getInput failure");
    exit(1);
  }

  // BoundedBuffer into which processed data w/o newlines are stored
  BoundedBuffer newLinesProcessed;
  createBuffer(&newLinesProcessed, MAX_BUFF_SIZE);
  
  // create binary semaphore for use with newline buffer
  semaphore nl_sem;
  createSem(&nl_sem, 1);

  ThreadInit nlToSpaceThread = {
    &theInput, // processes input from the BoundedBuffer theInput
    &newLinesProcessed, // puts processed data into BB newLinesProcessed
    &input_sem, // mutual exclusion for input buffer
    &nl_sem // mutual exclusion for processed newlines buffer
  };

  //create newline processing thread
  if (st_thread_create(newlineToSpace, &nlToSpaceThread,0,0) == NULL){
    perror("st_thread_create for newlineToSpace failure");
    exit(1);
  }

  // BoundedBuffer into which processed data after asterisks are handled go
  BoundedBuffer caratsProcessed;
  createBuffer(&caratsProcessed, MAX_BUFF_SIZE);
  
  // create binary semaphore for use with carat buffer
  semaphore carat_sem;
  createSem(&carat_sem, 1);

  ThreadInit caratThread = {
    &newLinesProcessed, // buffer from wich input is processed
    &caratsProcessed, // puts processed data into BB caratsProcessed
    &nl_sem, // for mutual exclusion in previous buffer
    &carat_sem // for mutual exclusion in processed carat buffer
  };

  // create asterisks to carat processing thread
  if (st_thread_create(asterisksToCarat, &caratThread,0,0) == NULL){
    perror("st_thread_create for newlineToSpace failure");
    exit(1);
  }

  ThreadInit printThread = {
    &caratsProcessed, // buffer from which input is processed
    NULL, // no output buffer, just printing to stdout
    &carat_sem, // for mutual exclusion in previous buffer
    NULL // no mutex needed, just printing to stdout
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
    down(init->to_bin_sem);
    theCharacter = getchar();
    produce(init->to, &theCharacter);
    count++;
    up(init->to_bin_sem);
  }

  // lets next buffer know when to stop consuming
  down(init->to_bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->to_bin_sem);

  // done, exit thread
  st_thread_exit(NULL);
}

// thread to change newlines to spaces
void *newlineToSpace(void *t){
  ThreadInit *init = t;
  char theCharacter;
  char space = ' '; // used for produce() in the case it needs a space
  int toBreak = 0; // toBreak is boolean initialized to FALSE

  while(toBreak == 0) {
    //if "from" is done inserting, and there are no elements, break
    down(init->from_bin_sem);
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0)){
        toBreak = 1;
      }
    }

    if (toBreak == 0){ // do only if toBreak is FALSE
      theCharacter = consume(init->from);
      down(init->to_bin_sem);
      if (theCharacter == '\n'){
        produce(init->to, &space);
      }
      else{
        produce(init->to, &theCharacter);
      }
      up(init->to_bin_sem);
    }
    up(init->from_bin_sem);
  }

  // lets next buffer know when to stop consuming
  down(init->to_bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->to_bin_sem);

  //done, exit thread
  st_thread_exit(NULL);
}


//thread to change asterisks to carats
void *asterisksToCarat(void *t){
  ThreadInit *init = t;
  char theCharacter;
  char nextCharacter;
  char carat = '^'; //used for produce() if it needs a carat
  int toBreak = 0;

  while(toBreak == 0) {
    //if from is done inserting and there are no elements, break
    down(init->from_bin_sem);
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0))
        toBreak = 1;
    }

    if (toBreak == 0){ 
      up(init->from_bin_sem);
      theCharacter = consume(init->from);
      if (theCharacter == '*'){
        nextCharacter = consume(init->from);
        if (nextCharacter == '*'){ // if two successive asterisks
          down(init->to_bin_sem);
          produce(init->to, &carat);
        }
        else { // if they're not both asterisks
          down(init->to_bin_sem);
          produce(init->to, &theCharacter);
          produce(init->to, &nextCharacter); 
        }
      }
      else { //the character was not an asterisk
        down(init->to_bin_sem);
        produce(init->to, &theCharacter);
      }
      up(init->to_bin_sem);
    }
    up(init->from_bin_sem);// releases from if toBreak == 1
  }

  // lets next buffer know when to stop consuming
  down(init->to_bin_sem);
  *(init->to->doneInserting) = 1;
  up(init->to_bin_sem);

  // done, exit thread
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
    down(init->from_bin_sem);
    if( *(init->from->doneInserting) == 1){
      if( (*init->from->numElements == 0)){
        toBreak = 1;
      }
    }

    if (toBreak == 0){
      up(init->from_bin_sem);
      //only call consume if there are elements to be consumed
      if(*(init->from->numElements) != 0){ 
        theCharacter = consume(init->from);
        //puts character in the buffer to print
        if(theCharacter != EOF){
          toPrint[index] = theCharacter;
          index++;
        }
        if (index == LINE_TO_PRINT_SIZE){
          toPrint[80] = '\0'; // terminates string
          printf("%s\n", toPrint);
          toPrint[0] = '\0'; // resets string
          index = 0;
        }
      }
    }
  }

  // done, exit thread
  st_thread_exit(NULL);
}
