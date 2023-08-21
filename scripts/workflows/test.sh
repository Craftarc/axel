#!/bin/bash

echo ">> Inside container"
set -euo pipefail

# Set CMake arguments here
cmake_args='-G Ninja -DAXEL_TEST=YES -DCMAKE_BUILD_TYPE=Debug'

## Compile and build
mkdir -p build
cmake -B build -S . ${cmake_args} # CMake configuration setup
cmake --build build
echo ">> Compilation complete"

# Run test executable
build/tests
