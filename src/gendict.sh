#!/bin/bash

gcc gendict.c -o gendict stemming.c -g3 -O0 -Wall -lqdbm
gcc gendict_stopword.c -o gendict_stopword stemming.c -g3 -O0 -Wall -lqdbm

ls ../data/*.raw | grep -v STOP | cut -d '.' -f 3 | awk '{print "./gendict .."$1".raw .."$1".DICT"}' | sh

