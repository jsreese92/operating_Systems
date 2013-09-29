#include <stdio.h>
#include "buffer.h"

//TODO
#define MAX_BUFF_SIZE 100

main(){
  //initialize the ST library runtime, necessary for up() and down()
  st_init();

  BoundedBuffer fromStdin;
  createBuffer(&fromStdin, MAX_BUFF_SIZE);

  char *p;
  *p = 'a';
  printf("fromStdin size: %d\n", *fromStdin.bufferSize);
  produce(&fromStdin, p);
  char fromConsume = consume(&fromStdin);
  printf("fromConsume:[%c]\n", fromConsume);

  printf("fromStdin.nextIn = %d\n", fromStdin.nextIn);
  printf("fromStdin.nextOut = %d\n", fromStdin.nextOut);



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
