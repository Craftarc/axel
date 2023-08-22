#!/bin/bash

echo ">> Inside container at: $(pwd)"
set -euo pipefail

# Set CMake arguments here
cmake_args='-G Ninja -DAXEL_TEST=YES -DCMAKE_BUILD_TYPE=Debug'
echo "CMake flags: "${cmake_args}""

## Compile and build
mkdir -p build
cmake -B build -S . ${cmake_args} # CMake configuration setup
cmake --build build
echo ">> Compilation complete"

# Run test executable
build/tests
