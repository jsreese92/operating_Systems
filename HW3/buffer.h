#include "st.h"
#include "semaphore.h"

typedef struct {
    int *maxBufferSize;
    int *numElements; // current number of elements buffer has
    int *doneInserting; // flag to know if insertion is complete on buffer
    semaphore *fullBuffers;
    semaphore *emptyBuffers;
    char *charBuffer; 
    int nextIn;
    int nextOut;
} BoundedBuffer;

void buffDeposit(BoundedBuffer *b);
void buffRemove(BoundedBuffer *b);
void createBuffer(BoundedBuffer *b, int theBuffSize);
