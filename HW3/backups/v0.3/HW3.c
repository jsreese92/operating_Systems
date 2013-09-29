#include <stdio.h>
#include "buffer.h"

//TODO
#define MAX_BUFF_SIZE 100

main(){
  //initialize the ST library runtime, necessary for up() and down()
  st_init();

  BoundedBuffer readInput;
  createBuffer(&readInput, MAX_BUFF_SIZE);

  printf("readInput size: %d\n", *readInput.bufferSize);

  buffDeposit(&readInput);
  buffRemove(&readInput);



/*
  int theBufferSize = MAX_BUFF_SIZE;
  semaphore fullSem;
  createSem(&fullSem, 0); //0 full buffers
  semaphore emptySem;
  createSem(&emptySem, MAX_BUFF_SIZE); //n empty buffers
  char theBuffer[MAX_BUFF_SIZE];

  BoundedBuffer theBoundedBuffer = {
    &theBufferSize,
    &fullSem,
    &emptySem,
    theBuffer
  };

  //have to derefernce the bufferSize pointer
  printf("theBoundedBuffer size: %d\n", (*theBoundedBuffer.bufferSize));

  int i;
  for(i = 0; i < 3; i++){
    up(theBoundedBuffer.fullBuffers);
    printf("up called\n");
  }
    for(i = 0; i < 3; i++){
      down(theBoundedBuffer.emptyBuffers);
      printf("down called\n");
  } 
*/

}
