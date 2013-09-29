#!/bin/bash

for file in ./testFiles/*.txt ; do
    toEcho=file: 
    echo $toEcho$file
    HW4 < $file
done
