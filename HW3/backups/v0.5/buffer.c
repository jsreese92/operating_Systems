#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void produce(BoundedBuffer *this, char *c){
//  printf("in produce\n");

  down((this->emptyBuffers));

//  printf("in buffer code\n");
  this->charBuffer[this->nextIn] = *c; //dereference c
//  printf("nextIn: %d\n", this->nextIn);
//  printf("bufferSize: %d\n", *(this->bufferSize));

  (this->nextIn) = (((this->nextIn)+1) % *(this->bufferSize)); 

  up((this->fullBuffers));
//  printf("exiting produce\n");
}

//returns character from given BoundedBuffer
char consume(BoundedBuffer *this){
//  printf("in consume\n");
  down((this->fullBuffers));

//  printf("in buffer code\n");
  char data = this->charBuffer[this->nextOut];
  this->nextOut = ((this->nextOut+1) % *(this->bufferSize));

  up((this->emptyBuffers));

  //consume data (push into next buffer)
//  printf("exiting consume\n");
  return data;
}

void createBuffer(BoundedBuffer *this,  int theBuffSize){
  int *allocateSize = malloc(sizeof(int));
  this->bufferSize = allocateSize;
  *(this->bufferSize) = theBuffSize;

  semaphore *fullBuffersSem = malloc(sizeof(semaphore));
  createSem(fullBuffersSem, 0);
  this->fullBuffers = fullBuffersSem;

  semaphore *emptyBuffersSem = malloc(sizeof(semaphore));
  createSem(emptyBuffersSem, theBuffSize);
  this->emptyBuffers = emptyBuffersSem;

  semaphore *theBinSem = malloc(sizeof(semaphore));
  createSem(theBinSem, 1); //binary semaphore initialized to 1
  this->bin_sem = theBinSem;

  char *theCharBuffer = malloc(theBuffSize * sizeof(char)) ;
  this->charBuffer = theCharBuffer;

  this->nextIn = 0;
  this->nextOut = 0;
}

  

