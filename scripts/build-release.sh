#!/bin/bash
# Build script
# Run this in the root directory of the project

# CMake settings group for production
cmake_args="-G Ninja -DCMAKE_BUILD_TYPE=Release -DAXEL_TEST=0"

echo ">> Current working directory: $(pwd)"

echo ">> Building Axel..."
mkdir -p build
rm -f build/CMakeCache.txt # Clean out previous build settings
cmake -B build -S . ${cmake_args} # CMake configuration setup
ninja -C build
echo ">> Compilation complete"
