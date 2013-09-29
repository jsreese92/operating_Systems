#!/bin/bash

for file in ./testFiles/*.txt ; do
    toEcho=file: 
    echo $toEcho$file
    warmup < $file
done

#for file in siml/*.siml ; do
#    #basename function returns prefix before .siml
#    prefix=`basename $file .siml`
#    allSpark priv $file $prefix.sas
#    done
                                                                 
