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
#define COMMANDS_MAX 3

char** getArgs (char* theInput, int* theNumArgs){
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
    *theNumArgs = argNum;
    return toReturn;
}

int main(){
    char input [INPUT_SIZE_MAX] = "";
    //char command [INPUT_SIZE_MAX] = "";

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
        while (x != EOF){
            x = getchar(); 
            if (x == '\n'){
                if (commandCount < COMMANDS_MAX){
                    input[input_index] = x;
                    strcpy(commands[commandCount], input);
                    commands[commandCount][input_index + 1] = '\0';
                    commandCount++;
                    input_index = 0;
                    printf("Prompt %% ");
                }
                else{
                    printf("Max number of commands exceeded, no more will be processed\n");
                }
            }  
            else{
                if (input_index <= INPUT_SIZE_MAX){
                    input[input_index] = x;
                    input_index++;
                }
                else
                    printf("maximum input size exceeded\n");
            }
        }
        input[input_index -1] = '\0';

        printf("input:%s\n",input);
        int j;
        for (j = 0; j < commandCount; j++)
            printf("commands[%d]=%s\n", j, commands[j]);
    }
/*
    while(!feof(stdin)){ 
        //output prompt
        printf("530shell()%% ");
        //read line from stdin stopping at newline
        //fgets (input, sizeof(input), stdin);

        if(!feof(stdin)){

            //fork a new process
            childPID = fork();

            if(childPID >= 0) { //fork successful
            
                if(childPID == 0) { //child process
                    //parsing input into arguments
                    //numArgs is useless
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
    }
    printf("\n"); //so regular shell prompt is put where it should be
    */
return 0;
}
