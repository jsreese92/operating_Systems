/* 
 * Since the main process (the daemon) is intended to be continuously
 * available, it has no defined termination condition and must be
 * terminated by an external action (Ctrl-C or kill command).
 *
 * The server has one parameter: the port number that will be used
 * for the "welcoming" socket where the client makes a connection.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 
 * Additional include and define statements for ForkExecRedirect process
 */
#include <errno.h>
#include <unistd.h>

#define MAX_TMP 100
#define MAX_ARGS 4  /* allows program name + 3 positional parameters */
#define MIN_ARGS 2  /* must have at least 1 positional parameter */
#define NUM_PARMS 4  /* allows program name + 2 parameters + NULL */
#define ERR_RETURN -1
#define INPUT_SIZE_MAX 10000
#define COMMANDS_MAX 1000

#include "Socket.h"
#include "ToUpper.h" /* definitions shared by client and server */


/* variables to hold socket descriptors */
ServerSocket welcome_socket;
Socket connect_socket;

void ForkExecuteRedirect_service(void);
void ForkExecuteRedirect_helper(int argc, char *argv[]);
char** getArgs(char* theInput, int* theNumberArgumentsReturned);

// Main server process that continuously runs until cntrl-c
int main(int argc, char* argv[])
{
  pid_t spid, term_pid; /* pid_t is typedef for Linux process ID */ 
  int chld_status;
  bool forever = true;

  if (argc < 2)
  {
    printf("No port specified\n");
    return (-1);
  }

  /* create a "welcoming" socket at the specified port */
  welcome_socket = ServerSocket_new(atoi(argv[1]));
  if (welcome_socket < 0)
  {
    printf("Failed new server socket\n");
    return (-1);
  }

  /* The daemon infinite loop begins here; terminate with external action*/
  while (forever)
  {
    /* accept an incoming client connection; blocks the
     * process until a connection attempt by a client.
     * creates a new data transfer socket.
     */
    connect_socket = ServerSocket_accept(welcome_socket);
    if (connect_socket < 0)
    {
      printf("Failed accept on server socket\n");
      exit (-1);
    }
    spid = fork();  /* create child == service process */
    if (spid == -1) 
    {
      perror("fork"); 
      exit (-1);
    }
    if (spid == 0) 
    {/* code for the service process */
      ForkExecuteRedirect_service();
      Socket_close(connect_socket);
      exit (0);
    } /* end service process */
    else /* daemon process closes its connect socket */
    {
      Socket_close(connect_socket);
      /* reap a zombie every time through the loop, avoid blocking*/
      term_pid = waitpid(-1, &chld_status, WNOHANG);
    }
  } /* end of infinite loop for daemon process */
}

// Sets up the service, ForkExecuteRedirect helper actually execs code
void ForkExecuteRedirect_service(void)
{
  int i, c, rc;
  int count = 0;
  bool forever = true;

  char input [INPUT_SIZE_MAX] = "";
  int numArguments = 0;
  pid_t childPID;
  char** arguments = NULL;
  int input_index = 0;

  /* will not use the server socket */
  Socket_close(welcome_socket);

  while (forever)  /* actually, until EOF on connect socket */
  {
    /* get a null-terminated string from the client on the data
     * transfer socket up to the maximim input line size.  Continue 
     * getting strings from the client until EOF on the socket.
     */ 
    for (i = 0; i < MAX_LINE; i++)
    {
      c = Socket_getc(connect_socket);
      if (c == EOF)
      {
        printf("Socket_getc EOF or error\n"); 
        return; /* assume socket EOF ends service for this client */           
      }
      else
      {

        if (c == '\n'){ // command entered
          input[input_index] = ' ';
          input[input_index + 1] = '\0'; // null-terminates string
          //printf("input: [%s]\n", input);
          arguments = getArgs(input, &numArguments);
          ForkExecuteRedirect_helper(numArguments, arguments);
        }

        else{ // command not entered, just continue filling input buffer
          if (input_index <= INPUT_SIZE_MAX){
            input[input_index] = c;
            input[input_index + 1] = '\0';
            input_index++;
          }
          else // if input size too large
            printf("max input size reached, no more input will be processed\n");
        }

      }
    }
  } /* end while loop of the service process */
  return;
}

// actually does the execution and reports back to ForkExecuteRedirect
void ForkExecuteRedirect_helper(int argc, char *argv[]){
  pid_t cpid, term_pid; /* type for Linux process id */
  int chld_status; 
  int i;
  int execCheck;
  char *parms_array[NUM_PARMS];  /* arrary of pointers to strings */
  unsigned char new_line[MAX_LINE];
  unsigned char tmp_name[MAX_TMP];
  unsigned char id_str[MAX_TMP];
  int id;

  /* variable names for file "handles" */
  FILE *tmpFP;
  FILE *fp;

  /* The positional parameters passed to this program as pointers to 
   * strings can just be passed on to execvp which requires a
   * NULL-terminated array of pointers to strings.  By convention, the
   * first entry in the array is the file name to be executed, and the
   * next non-null pointers are for positional parameters to that program.
   */

  /* get the parent process ID and use it to create a file name for the
   * temporary file with the format "tmpxxxxx" where xxxxx is the ID 
   */

  id = (int) getpid();
  sprintf(id_str, "%d", id);
  strcpy(tmp_name,"tmp");
  strcat(tmp_name, id_str);    

  /* create child process, report error if present and exit */  
  cpid = fork(); 
  if (cpid == -1) 
  {
    perror("fork"); /* perror creates description of OS error codes */
    exit(ERR_RETURN); 
  }

  /* conditional execution of statements depending on whether the code is
   * if running as the parent or child process.
   */

  if (cpid == 0) 
  { /* the child process */
    /* dynamically redirect stdout to the named temporary 
     * file open for writing
     */ 
    fp = freopen(tmp_name, "w", stdout); 

    /* do the OS execvp() call, if there is an error return, report it.  Otherwise
     * this call will not return and the new image will execute as this process ID.
     */

    execCheck = execvp(argv[0], argv);
 
    if (execCheck == -1) 
    {
      perror("execvp, execCheck == -1"); 
      exit(-1); 
    }
  }  /* end of code that can be executed by the child process */
  else 
  {/* code executed by the parent process */
    /* wait for the child to terminate and determine its exit status */
    /* this is necessary to prevent the accumulation of "Zombie" processes */

    term_pid = waitpid(cpid, &chld_status, 0);
    if (term_pid == -1) 
      perror("waitpid"); 
    else
    {
      if (WIFEXITED(chld_status)) 
        printf("PID %d exited, status = %d\n", cpid, WEXITSTATUS(chld_status));
      else
        printf("PID %d did not exit normally\n", cpid);
    }

    if ((tmpFP = fopen (tmp_name, "r")) == NULL) {
      fprintf (stderr, "error opening tmp file\n");
      exit (-1);
    }
    while (!feof (tmpFP)) {
      /* Get line from file */

      if (fgets (new_line, sizeof(new_line), tmpFP) == NULL)
        break;
      int toClientCount = strlen(new_line);
      int new_index;
      char toClientChar;
      int new_rc;
      for (new_index = 0; new_index < toClientCount; new_index++){
        toClientChar = new_line[new_index];
        new_rc = Socket_putc(toClientChar, connect_socket);
        if (new_rc == EOF){
          printf("Socket_putc EOF or error\n");             
          Socket_close(connect_socket);
          exit (-1);  // assume socket problem is fatal, end client
        }
      }
    }
    // delete the temporary file 
    remove(tmp_name);
  } // end of code that can be executed by the parent process
  exit(0);
}

//takes a string and splits it into arguments
char** getArgs (char* theInput, int* numArgsReturned){
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
  *numArgsReturned = argNum;
  return toReturn;
}
