#!/bin/bash

PATH_TO=${HOME}/parallel-skiplist

set -o xtrace   # print cmds
set -e          # errors

make clean
make "test"

echo $'\n===== Testing for correctness... =====\n'
${PATH_TO}/tests/test

make
echo $'\n===== Benchmarking performance of skiplist... =====\n'


${PATH_TO}/src/main
