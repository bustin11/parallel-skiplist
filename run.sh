#!/bin/bash
# to be used with the top level makefile that i made my own, not with cmake
PATH_TO=${HOME}/parallel-skiplist

set -o xtrace   # print cmds
set -e          # errors

make clean

if [ "$1" != "main" ]
then

    make "test"
    echo $'\n===== Testing for correctness... =====\n'
    ${PATH_TO}/tests/test

fi

if [ "$1" != "test" ]
then

    make
    echo $'\n===== Benchmarking performance of skiplist... =====\n'
    ${PATH_TO}/src/main -f "output.txt"

fi
