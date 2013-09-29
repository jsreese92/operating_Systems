#include <stdio.h>
#include "buffer.h"

#define MAX_BUFF_SIZE 1024

main(){
  BoundedBuffer readInput;
  createBuffer(&readInput, MAX_BUFF_SIZE);

  buffDeposit(&readInput);
  buffRemove(&readInput);
}
