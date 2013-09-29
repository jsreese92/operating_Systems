#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 512

//CORRECT
void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

int main()
{
    char input[MAX_STRING_SIZE]="";
    FILE* pFile;
    pFile = stdin;
    if (pFile!=NULL)
    {
        int x;
        while((x = fgetc(pFile)) != EOF )
        {
            //replaces newlines with spaces
            if (x == '\n')
                x = ' ';
            append(input, x);
        }    
    }
    int numOfLines = (strlen(input) / 80);
//printf("Number of 80 character lines: %d\n", numOfLines);

/***FROM POPULATE LINES FUNCTION***/
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

    //not sure why this puts what linesToPrint[1] should be...
    //ASK DR. JEFFAY ABOUT PASSING POINTERS VS. ACTUAL OBJECTS TO
    //ARRAYS, it's messing up because I'm changing the pointers each
    //time and I don't know how to pass the actual value...

    //printf("linesToPrint[0]= %s\n", linesToPrint[0]);
/***ABOVE FROM POPULATE LINES FUNCTION***/

    //length is 1 more than number of characters b/c of EOF character
return 0;
}
