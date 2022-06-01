#!/bin/bash

set -e
for i in {0..1000000}
do
src/main -N 2
done