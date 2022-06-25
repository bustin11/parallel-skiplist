#!/bin/bash

PATH_TO=${HOME}/parallel-skiplist

set -o xtrace   # print cmds
set -e          # errors

make clean

if [ "$1" != "main" ]
then

    make "test"
    echo $'\n===== Testing for correctness... =====\n'
    ${PATH_TO}/tests/test -N 4

fi

if [ "$1" != "test" ]
then

    make
    echo $'\n===== Benchmarking performance of skiplist... =====\n'
    echo $'threads=1' >> 'output.txt'
    ${PATH_TO}/src/main -N 1 -f "output.txt"
    echo $'threads=2' >> 'output.txt'
    ${PATH_TO}/src/main -N 2 -f "output.txt"
    echo $'threads=4' >> 'output.txt'
    ${PATH_TO}/src/main -N 4 -f "output.txt"

fi
