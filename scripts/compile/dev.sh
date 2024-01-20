#!/bin/bash

# Absolute path to project root
dir=$(realpath "$(dirname "${BASH_SOURCE[0]}")/../..")

# CMake settings group for development
cmake_args="-G Ninja -DCMAKE_BUILD_TYPE=Debug -DAXEL_TEST=1 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=1"

cd build
cmake .. ${cmake_args}
ninja
cd ..

build/main