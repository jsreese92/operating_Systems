#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

//produce
void buffDeposit(BoundedBuffer *b){
  printf("in deposit\n");
  down((b->emptyBuffers));

  printf("in buffer code\n");
  //insert buffer code here

  up((b->fullBuffers));
  printf("exiting deposit\n");
}

//consume
void buffRemove(BoundedBuffer *b){
  printf("in remove\n");
  down((b->fullBuffers));

  printf("in buffer code\n");
  //insert buffer code here

  up((b->emptyBuffers));
  printf("exiting remove\n");
}

void createBuffer(BoundedBuffer *b,  int theBuffSize){
  b->bufferSize = &theBuffSize;

  semaphore *fullBuffersSem = malloc(sizeof(semaphore));
  createSem(fullBuffersSem, 0);
  b->fullBuffers = fullBuffersSem;

  semaphore *emptyBuffersSem = malloc(sizeof(semaphore));
  createSem(emptyBuffersSem, theBuffSize);
  b->emptyBuffers = emptyBuffersSem;

  char charBuffer[theBuffSize];
  b->theBuffer = charBuffer;
}

  

