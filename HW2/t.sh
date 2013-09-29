#!/bin/bash

for file in ./testFiles/*.txt ; do
    toEcho=file: 
    echo $toEcho$file
    530shell < $file
done
