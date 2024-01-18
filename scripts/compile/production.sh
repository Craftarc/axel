#!/bin/bash

# Absolute path to project root
root=$(realpath "$(dirname "${BASH_SOURCE[0]}")")/../..

# CMake settings group for production
cmake_args="-G Ninja -DCMAKE_BUILD_TYPE=Release -DAXEL_TEST=0"

echo ">> Building Axel..."
mkdir -p "${root}"/build
rm -f "${root}"/build/CMakeCache.txt # Clean out previous build settings
cmake -B "${root}"/build -S "${root}" ${cmake_args} # CMake configuration setup
ninja -C "${root}"/build main
echo ">> Compilation complete"
