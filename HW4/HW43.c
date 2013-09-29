#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// asterisk handler
main(){
  int x;
  int y;
  while (x != EOF){
    x = getchar();
    if (x == '*'){
      y = getchar();
      if ( y != EOF){
        if ( y == '*'){ // if both asterisks
          printf("^");
        }
        else { // only x is an asterisk
          printf("%c", x);
          printf("%c", y);
        }
      }
    }
    else { // x is not an asterisk
      printf("%c", x);
    }
  }
}
