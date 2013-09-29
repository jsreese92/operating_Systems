#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#define INPUT_SIZE_MAX 10000
#define COMMANDS_MAX 100

//takes a string and splits it into "words" for use as arguments
char** getArgs (char* theInput){
    //trim leading space
    while(isspace(*theInput)){
        theInput++;
    }

    //initialize toReturn
    char** toReturn = malloc(COMMANDS_MAX * sizeof(char*));
    int i;
    for (i = 0; i < COMMANDS_MAX; i++){
        toReturn[i] = malloc(INPUT_SIZE_MAX * sizeof(char));
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
    return toReturn;
}

int main(){
    char input [INPUT_SIZE_MAX] = "";

    //initialize commands
    char** commands = malloc(COMMANDS_MAX * sizeof(char*));
    int i;
    for (i = 0; i < COMMANDS_MAX; i++){
        commands[i] = malloc(INPUT_SIZE_MAX * sizeof(char));
    }

    pid_t childPID;

    int input_index = 0;
    int x;
    int commandCount = 0;
    printf("Prompt %% ");
    while (x != EOF){
        x = getchar(); 
        // if command is entered
        if (x == '\n'){
            if (commandCount < COMMANDS_MAX){
                input[input_index] = x;
                strcpy(commands[commandCount], input);
                commands[commandCount][input_index + 1] = '\0';
                input_index = 0;
                //prints prompt after newline is entered
                printf("Prompt %% ");

                //fork a new process
                childPID = fork();

                if(childPID >= 0) { //fork successful
                
                    if(childPID == 0) { //child process
                        //parsing input into arguments
                        char** arguments = getArgs(commands[commandCount]);

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
            commandCount++;
            }
            else{
                printf("Max number of commands exceeded, no more will be processed\n");
            }
        }  
        //populates input string as long as it's less than the max
        else{
            if (input_index <= INPUT_SIZE_MAX){
                input[input_index] = x;
                input_index++;
            }
            else
                printf("maximum input size exceeded, no further input will be processed\n");
        }
    }

    //resets input string for use again
    input[input_index -1] = '\0';
return 0;
}
