#!/bin/bash

for file in ./testFiles/*.txt ; do
  toEcho=file:
  echo $toEcho$file
  HW41 < $file | HW42 | HW43 | HW44
done
