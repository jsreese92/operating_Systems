#include "st.h"
#include "semaphore.h"

typedef struct {
    int *bufferSize;
    semaphore *fullBuffers;
    semaphore *emptyBuffers;
    semaphore *bin_sem;
    char *charBuffer; 
    int nextIn;
    int nextOut;
} BoundedBuffer;

void buffDeposit(BoundedBuffer *b);
void buffRemove(BoundedBuffer *b);
void createBuffer(BoundedBuffer *b, int theBuffSize);
