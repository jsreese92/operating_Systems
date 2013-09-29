#!/bin/bash

gcc ToUpperDaemon.c libsocket.c -o server_jeffay
gcc ToUpperClient.c libsocket.c -o client_jeffay
gcc ForkExecRedirect.c -o shell_jeffay
