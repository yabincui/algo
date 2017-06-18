#!/bin/bash

set -ex

rm -rf out
mkdir out
cd out
cmake -G "Unix Makefiles" ..
make
test/algo_test