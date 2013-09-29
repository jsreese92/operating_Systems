#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// newline handler
main(){
  int x;
  while (x != EOF){
    x = getchar();
    if ( x == '\n')
      printf(" ");
    else
      printf("%c", x);
  }
}
  


