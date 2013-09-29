#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// get input
main(){
  int x;
  while (x != EOF){
    x = getchar();
    printf("%c", x);
  }
}
