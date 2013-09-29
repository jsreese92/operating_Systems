#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void produce(BoundedBuffer *this, char *c){
  down((this->emptyBuffers));

  this->charBuffer[this->nextIn] = *c; //dereference c

  (this->nextIn) = (((this->nextIn)+1) % *(this->maxBufferSize)); 

  //increments current number of elements in buffer
  *(this->numElements) = *(this->numElements) + 1;
  printf("in produce function: numElements = [%d]\n", *this->numElements);

  up((this->fullBuffers));
}

// returns character from given BoundedBuffer
char consume(BoundedBuffer *this){
  down((this->fullBuffers));

  char data = this->charBuffer[this->nextOut];
  this->nextOut = ((this->nextOut+1) % *(this->maxBufferSize));
  
  //decrements current number of elements in buffer
  *(this->numElements) = *(this->numElements) -1;

  up((this->emptyBuffers));
  return data;
}

void createBuffer(BoundedBuffer *this,  int theBuffSize){
  int *allocateSize = malloc(sizeof(int));
  this->maxBufferSize = allocateSize;
  *(this->maxBufferSize) = theBuffSize;

  //note that this includes EOF!
  int *allocateNumElements = malloc(sizeof(int));
  this->numElements = allocateNumElements;
  *(this->numElements) = 0;

  int *allocateDoneInserting = malloc(sizeof(int));
  this->doneInserting = allocateDoneInserting;
  *(this->doneInserting) = 0; //initialize doneInserting flag to FLASE

  semaphore *fullBuffersSem = malloc(sizeof(semaphore));
  createSem(fullBuffersSem, 0);
  this->fullBuffers = fullBuffersSem;

  semaphore *emptyBuffersSem = malloc(sizeof(semaphore));
  createSem(emptyBuffersSem, theBuffSize);
  this->emptyBuffers = emptyBuffersSem;

  char *theCharBuffer = malloc(theBuffSize * sizeof(char)) ;
  this->charBuffer = theCharBuffer;

  this->nextIn = 0;
  this->nextOut = 0;
}
