#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 1024

//I know I need to have a lot of code factored out of my main
//method, but I was having trouble with malloc'ing and whatnot
//in order to access things I did in my methods, so I'm planning
//on coming in to office hours for some malloc and function help

//appends a character onto a string
void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

int main()
{
    char input[MAX_STRING_SIZE]="";
    int x;
    int y;
    while((x = fgetc(stdin)) != EOF )
    {
        //replaces newlines with spaces
        if (x == '\n'){
            x = ' ';
            append(input,x);
        }
        //handles asterisks
        if (x == '*'){
            y = fgetc(stdin);
            if(y== '*'){
                append(input, '^');
            }
            else {
                append(input, x);
                append(input, y);
            }

        }
        else{
            append(input, x);
        }
    }    
    int numOfLines = (strlen(input) / 80);

//printf("Number of 80 character lines: %d\n", numOfLines);
//printf("input: %s\n", input);

/***BELOW FROM POPULATE LINES FUNCTION***/
    int inputPointer;
    
    char linesToPrint[numOfLines][80]; //array of strings

    int i =0;
    while(i < numOfLines){
        for(inputPointer=0; inputPointer < 80; inputPointer++){
           linesToPrint[i][inputPointer] = input[inputPointer + (i * 80)];
        }
    linesToPrint[i][80] = '\0';
    printf("%s\n", linesToPrint[i]);
    i++;
    }
/***ABOVE FROM POPULATE LINES FUNCTION***/

return 0;
}
