#!/bin/bash

gcc HW5server.c libsocket.c -o server_j
gcc HW5client.c libsocket.c -o client_j
gcc ForkExecRedirect.c -o shell_j
