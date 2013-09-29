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

//contains each line of output to be printed to screen
struct Lines{
    char** theLines; //array of strings
};

struct Lines* allocateLines(){
    struct Lines* l = malloc(sizeof(*l));

    return(l);
}

struct Lines* lines_init(){
    struct Lines* lines = (struct Lines*)malloc(sizeof(struct Lines));
    lines->theLines = 0;
}

//CORRECT places strings into struct Lines
void populateLines(struct Lines* l, int num, char* theInput){
    int i =0;
    int inputPointer;
    
    char* linesToPrint[num]; //array of strings

    while(i < num){
        char lineToAdd[80];
        for(inputPointer=0; inputPointer < 80; inputPointer++){
//printf("theInput[inputPointer]: %c\n", theInput[inputPointer]);
           lineToAdd[inputPointer] = theInput[inputPointer];
//printf("lineToAdd[inputPointer]: %c\n", lineToAdd[inputPointer]);
        }
        lineToAdd[80] = '\0';
        linesToPrint[i] = lineToAdd;
//printf("linesToPrint[i]:, %s\n", linesToPrint[i]);
    i++;
    }
    l->theLines = linesToPrint;
printf("from popLines: l->theLines[0]= %s\n", l->theLines[0]);
}

struct Lines* popLines_j(struct Lines* l, int numLines, char* theInput){
    int i =0;
    int inputPointer;
    
    char* linesToPrint[numLines]; //array of strings

    while(i < numLines){
        char lineToAdd[80];
        for(inputPointer=0; inputPointer < 80; inputPointer++){
           lineToAdd[inputPointer] = theInput[inputPointer];
        }
        lineToAdd[80] = '\0';
        linesToPrint[i] = lineToAdd;
    i++;
    }
    l->theLines = linesToPrint;
printf("from popLines_j: l->theLines[0]= %s\n", l->theLines[0]);
//    l.theLines = linesToPrint;
//printf("from popLines_j: l.theLines[0]= %s\n", l.theLines[0]);
    return (l);
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
/*******ABOVE WORKS FINE**************************************/
    

/***FROM POPULATE LINES FUNCTION***/
    //struct Lines* l = malloc(sizeof(*l));
    int inputPointer;
    
    char linesToPrint[numOfLines][80]; //array of strings

//printf("The Input: %s\n", input);

/*
    int i =0;
    while(i < numOfLines){
//printf("i=%d\n", i);
        char lineToAdd[80];
        for(inputPointer=0; inputPointer < 80; inputPointer++){
//printf("theInput[inputPointer]: %c\n", input[inputPointer]);
           lineToAdd[inputPointer] = input[inputPointer + (i * 80)];
//printf("lineToAdd[%d]: %c\n",inputPointer, lineToAdd[inputPointer]);
        }
    lineToAdd[80] = '\0';
    //"if you change what str points to down the line, it would also change
    //the value of myArray[3] from bookmark "string array"
    //so this points to the "lineToAdd" pointer regardless of what it is
    linesToPrint[i] = lineToAdd;
    printf("%s\n", linesToPrint[i]);
    i++;
    }
*/
   
    int i =0;
    while(i < numOfLines){
        for(inputPointer=0; inputPointer < 80; inputPointer++){
           linesToPrint[i][inputPointer] = input[inputPointer + (i * 80)];
        }
    linesToPrint[i][80] = '\0';
    printf("%s\n", linesToPrint[i]);
    i++;
    }

    /* 
    int j;
    for(j=0; j < numOfLines; j++){
        printf("linesToPrint[%d]= %s\n",j, linesToPrint[j]);
    }
    */

    //not sure why this puts what linesToPrint[1] should be...
    //printf("linesToPrint[0]= %s\n", linesToPrint[0]);
    //l->theLines = linesToPrint;

    /*
    int j;
    for(j=0; j < numOfLines; j++){
        printf("linesToPrint[%d]= %s\n",j, linesToPrint[j]);
    }
    */

/***ABOVE FROM POPULATE LINES FUNCTION***/

/* this produces output
    struct Lines* l = malloc(sizeof(*l));
    l = popLines_j(l, numOfLines, input); 

printf("from main: l->theLines[0]= %s\n", l->theLines[0]);

    int i;
    for(i=0; i < numOfLines; i++){
printf("into for, i = %d\n", i);
printf("from for: l->theLines[0]= %s\n", l->theLines[0]);
        printf("%s\n", l->theLines[i]);
    }
*/
    //length is 1 more than number of characters b/c of EOF character
return 0;
}
