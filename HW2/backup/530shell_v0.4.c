#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_SIZE_MAX 10000
#define ARGS_MAX 100
#define STRING_SIZE_MAX 100

//correct
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
    *theNumArgs = argNum;
    return toReturn;
}

int main(int argc, char *argv[]){
    char input [INPUT_SIZE_MAX];
    char exitKey[]="exit\n";
    pid_t childPID;

    //output prompt
    printf("Prompt:");
    //read line from stdin
    fgets (input, sizeof(input), stdin);

/*
    //array of arguments for exec, must be null-terminated
    char *newargv[] = {"ls", NULL};

    //execvp doesn't need the environment variable
    //why do I have to put "ls" twice in execvp?
        //The first "ls" finds where ls is (/bin/ls), then the second one
        //is what actually does the command (or vice-versa)

        if(execvp("ls",newargv) < 0){
            //execvp returns -1 if it fails
            printf("ERROR: exec failed\n");
            exit(EXIT_FAILURE);
        }
*/

    //fork a new process
    childPID = fork();

    if(childPID >= 0) { //fork successful
    
        if(childPID == 0) { //child process
            printf("In child process\n");
            //note that input inculdes the newline character
            printf("input is:%s", input);

            //parsing input into arguments
            int numArgs; //updated in getArgs function
            char** arguments = getArgs(input, &numArgs);
            int i;
            for(i = 0; i < numArgs; i++){
                printf("arguments[%d]=%s\n", i, arguments[i]);
            }

            printf("exiting child process\n");
        }
        
        else { //parent process
            printf("In parent process\n");

            int status; 
            do {
                //puts return value into status
                wait(&status);

                //wait returns the process ID of a child (greater than 0),
                //-1 on error, and errno is set to ECHILD when
                //there are no child processes to wait for
                if (status == -1 && errno != ECHILD){
                    perror("Error during wait()");
                    //not sure what this does yet
                    //abort(); 
                }
            } while (status > 0);
            printf("exiting parent process\n");
        }
    }

    else { //fork failed
        printf("\nFork failed!\n");
        return 1;
    }
    
    printf("program terminated\n");

return 0;
}
