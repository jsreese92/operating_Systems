#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_SIZE_MAX 10000
#define ARGS_MAX 100
#define STRING_SIZE_MAX 100

char** getArgs (char* theInput, int* theNumArgs){
    //trim leading space
    while(isspace(*theInput)){
        theInput++;
    }

    //initialize toReturn
    char** toReturn = malloc(ARGS_MAX * sizeof(char*));
    int i;
    for (i = 0; i < ARGS_MAX; i++){
        toReturn[i] = malloc(STRING_SIZE_MAX * sizeof(char));
    }

    int argNum = 0;
    int argPointer = 0;
    int length = strlen(theInput);
    for(i = 0; i < length; i++){
        if(isspace(theInput[i])){
            //end string with null terminator
            toReturn[argNum][argPointer] = '\0';
            argNum++;
            argPointer = 0;
            //skips over arbitrary amount of spaces
            while(isspace(theInput[i+1])){
                i++;
            }
        }
        else{
        toReturn[argNum][argPointer] = theInput[i];    
        argPointer++;
        }
    }
    //makes the last string null-terminated for execvp
    toReturn[argNum] = NULL;
    *theNumArgs = argNum;
    return toReturn;
}

int main(){
    char input [INPUT_SIZE_MAX];
    char exitKey[]="exit\n";
    pid_t childPID;

    while(!feof(stdin)){ 
        //output prompt
        printf("530shell() %% ");
        //read line from stdin
        fgets (input, sizeof(input), stdin);

        //fork a new process
        childPID = fork();

        if(childPID >= 0) { //fork successful
        
            if(childPID == 0) { //child process
                //parsing input into arguments
                int numArgs; //updated in getArgs function
                char** arguments = getArgs(input, &numArgs);

                if(execvp(arguments[0],arguments) < 0) {
                    printf("ERROR: exec failed\n");
                    exit(1);
                }
            }
        
            else { //parent process
                int status; 
                while (wait(&status) != childPID)
                    //NOOP
                    ;                
            }
        }

        else { //fork failed
            printf("\nFork failed!\n");
            return 1;
        }
    }
return 0;
}
