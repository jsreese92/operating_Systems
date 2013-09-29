#!/bin/bash

#creates unlinked buffer object file
gcc -c buffer.c -o buffer.o
#creates unlinked HW3 file
gcc -c HW3.c -o HW3.o
#links HW3, buffer, and semaphore while including ST
gcc HW3.o buffer.o semaphore.o -o HW3 -L. -lst
