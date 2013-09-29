#include <stdio.h>
#include "buffer.h"

#define MAX_BUFF_SIZE 5

main(){
  //initialize the ST library runtime, necessary for up() and down()
  st_init();

  /*
  BoundedBuffer readInput;
  createBuffer(&readInput, MAX_BUFF_SIZE);
  */

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
  printf("Buffer Size: %d\n", (*theBoundedBuffer.bufferSize));

  int i;
  for(i = 0; i < 10; i++){
    up(theBoundedBuffer.fullBuffers);
  }
    for(i = 0; i < 6; i++){
      down(theBoundedBuffer.emptyBuffers);
  } 






  /*
  int i;
  for(i = 0; i < MAX_BUFF_SIZE; i++){
    up(&readInput.emptyBuffers);
    emptyBuffersVal++;
  }

  for (i = 0; i < MAX_BUFF_SIZE; i++){
    down(&readInput.emptyBuffers);
    emptyBuffersVal--;
  }
  */


  //buffDeposit(&readInput);
  //buffRemove(&readInput);
}
