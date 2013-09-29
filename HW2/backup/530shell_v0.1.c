#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STRING_SIZE 512

//appends a character onto a string
void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

//correct
bool isWhitespace(char c){
    if (c == ' ' || c == '\t' || c == '\n')
        return true;
    else
        return false;
}

char** getArguments(char* input){
        
}

int main(){
    char input [MAX_STRING_SIZE];
    char exitKey[]="exit\n";
    pid_t childPID;
    
    //worry about doing it multiple times later (had a do while here)
    
    //output prompt
    printf("Prompt:");
    //read line from stdin
    fgets (input, sizeof(input), stdin);

    int length = strlen(input);
    int i;
    for(i = 0; i < length; i++){
        if (isWhitespace(input[i]))
            printf("whitespace!\n");
    }


    /*

    //fork a new process
    childPID = fork();

    if(childPID >= 0) { //fork successful
    
        if(childPID == 0) { //child process
            printf("In child process\n");
            //note that input inculdes the newline character
            printf("input is:%s", input);

            //parsing input into arguments
            

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
    
    */

    printf("program terminated\n");

return 0;
}
