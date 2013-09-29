#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

//produce
void buffDeposit(BoundedBuffer *b){
  printf("in deposit\n");
//  down(b->emptyBuffers);
  //insert buffer code here
//  up(b->fullBuffers);
}

//consume
void buffRemove(BoundedBuffer *b){
  printf("in remove\n");
//  down(b->fullBuffers);
  //insert buffer code here
//  up(b->emptyBuffers);
}

void createBuffer(BoundedBuffer *b,  int theBuffSize){
  b->bufferSize = theBuffSize;

  semaphore fullBuffersSem;
  createSem(&fullBuffersSem, 0);

  semaphore emptyBuffersSem;
  createSem(&emptyBuffersSem, theBuffSize);

  char a[theBuffSize];
  b->theBuffer = a;
}

  

