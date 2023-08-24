#!/bin/bash
# Build script for prod

# Set CMake arguments here
cmake_args='-G Ninja -DCMAKE_BUILD_TYPE=Release'

echo ">> Environment is: $1"
if [ "$1" = 'prod' ]; then
  cmake_args+=' -DAXEL_TEST=NO'
elif [ "$1" = 'test' ]; then # If 'env' is not defined then default to building for test environment
  cmake_args+=' -DAXEL_TEST=YES'
else
  echo "Invalid argument for 'env'. Exiting..."
  exit 1
fi

echo ">> Current working directory: $(pwd)"

echo ">> Building Axel..."
mkdir -p build
cmake -B build -S . ${cmake_args} # CMake configuration setup
cmake --build build --target all --target aws-lambda-package-main
echo ">> Compilation complete"

bash

