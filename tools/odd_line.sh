#!/bin/bash

i=0;
while read line;
do
if test $(($i%2)) -eq 0; then
    if test ${#line} -gt 48; then
    echo $line
    fi
fi
i=$(($i+1))
done

