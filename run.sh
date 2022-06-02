#!/bin/bash

PATH_TO=${HOME}/parallel-skiplist

set -o xtrace   # print cmds
set -e          # errors

make clean
make "test"

echo $'\n===== Testing for correctness... =====\n'
${PATH_TO}/tests/test -N 4

make
echo $'\n===== Benchmarking performance of skiplist... =====\n'

${PATH_TO}/src/main -N 1

${PATH_TO}/src/main -N 2

${PATH_TO}/src/main -N 4
