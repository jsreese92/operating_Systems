#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_TO_PRINT_SIZE 80

// output handler
main(){
  char toPrint[LINE_TO_PRINT_SIZE + 1] = ""; // +1 for null-termination
  int index = 0;
  int x;
  while (x != EOF){
    x = getchar();
    if (isascii(x)){
      toPrint[index] = x;
      index++;
    }
    if (index == LINE_TO_PRINT_SIZE){
      toPrint[80] = '\0'; // terminates string
      printf("%s\n", toPrint);
      toPrint[0] = '\0'; // resets string
      index = 0;
    }
  }
}
    
    

