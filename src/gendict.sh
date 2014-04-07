#!/bin/bash

gcc gendict.c -o gendict stemming.c -g3 -O0 -Wall -lqdbm
gcc gendict_stopword.c -o gendict_stopword stemming.c -g3 -O0 -Wall -lqdbm
gcc gendict_oxford.c -o gendict_oxford stemming.c -g3 -O0 -Wall -lqdbm
gcc qdbm_read.c -o qdbm_read stemming.c -g3 -O0 -Wall -lqdbm

ls ../data/*.raw | grep -v STOP | cut -d '.' -f 3 | awk '{print "./gendict .."$1".raw .."$1".DICT"}' | sh

