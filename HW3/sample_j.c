/********************
 * sample.c
 * COMP 530 Fall 2012
 * David Idol
 ********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "st.h"
#include "semaphore.h"

// default output stream
#define DEFAULT_OUT stdout
// default number of iterations to run for
#define DEFAULT_NUM_ITERS 50
// how long for each thread to sleep in microseconds
#define INC_SLEEP_TIME 333333 // 0.3 seconds
#define DEC_SLEEP_TIME 666666 // 0.6 seconds


// data structure passed to the each thread upon initialization
typedef struct {
	FILE *output_stream; // where to print
	semaphore *bin_sem; // pointer to a binary semaphore
	int *value_ptr; // pointer to the int value
} ThreadInit;

// function prototypes
void *increment_thread_func(void *s);
void *decrement_thread_func(void *s);

// creates two threads using ST, one of which increments a shared counter,
// and one of which decrements it. The counter's value is printed to stdout
// after each operation. After a certain number of operations the program
// terminates.
int main (int argc, char const *argv[]) {
	// initialize the ST library runtime
	st_init();
	
	// store an arbitrary integer
	int value = 0;
	
	// create a binary semaphore initialized to 1
	semaphore bin_sem;
	createSem(&bin_sem, 1);
	
	// which output stream to use
	FILE *output = DEFAULT_OUT;
	
	// create our init data struct
	ThreadInit thread_init = {
		output,
		&bin_sem,
		&value
	};

  //JSR
  int semValue = 1;
  int i;
  for (i = 0; i < 5; i++){
    semValue++;
    up(thread_init.bin_sem);
    printf("semValue= %d\n", semValue);
  }

  /*
	// create the increment thread
	if (st_thread_create(increment_thread_func, &thread_init, 0, 0) == NULL) {
		perror("st_thread_create for increment thread failure");
		exit(1);
	}
	
	// create the decrement thread
	if (st_thread_create(decrement_thread_func, &thread_init, 0, 0) == NULL) {
		perror("st_thread_create for decrement thread failure");
		exit(1);
	}
  */

	// main thread exits
	st_thread_exit(NULL);
	return 0;
}

// function called by the increment thread
// increments the counter, prints the current value, and then
// sleeps for some time.
void *increment_thread_func(void *s) {
	ThreadInit *init = s; // cast the void pointer to keep compiler happy
	
	int num_inc;

	for (num_inc = 0; num_inc < DEFAULT_NUM_ITERS; num_inc++) {
		// decrement the semaphore
		down(init->bin_sem);
	 	// dereference the value pointer, then increment it
		(*(init->value_ptr))++;
		// print the current value
		fprintf(init->output_stream, "%d\n", (*(init->value_ptr)));
		fflush(init->output_stream);
		// increment the semaphore
		up(init->bin_sem);
		// wait some time
		st_usleep(INC_SLEEP_TIME);
	}
	// all done - thread exits
	st_thread_exit(NULL);
}

// function called by the decrement thread
// decrements the counter, prints the current value, and then
// sleeps for some time.
void *decrement_thread_func(void *s) {
	ThreadInit *init = s; // cast the void pointer to keep compiler happy
	
	int num_dec;

	for (num_dec = 0; num_dec < DEFAULT_NUM_ITERS; num_dec++) {
		 // decrement the semaphore
		down(init->bin_sem);
		 // dereference the value pointer, then decrement it
		(*(init->value_ptr))--;
		// print the current value
		fprintf(init->output_stream, "%d\n", (*(init->value_ptr)));
		fflush(init->output_stream);
		// increment the semaphore
		up(init->bin_sem);
		// wait some time
		st_usleep(DEC_SLEEP_TIME);
	}
	// all done - thread exits
	st_thread_exit(NULL);
}


