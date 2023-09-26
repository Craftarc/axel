#!/bin/bash

# Call CMake to generate a fresh compile_commands.json for the clangd LSP
# To be called in craftarc/axel:dependencies

# Absolute path to project root directory
dir=$(realpath "$(dirname "${BASH_SOURCE[0]}")/..")
echo $dir

# Build args besides the one that generates compile_commands.json
cmake_args='-G Ninja -DCMAKE_BUILD_TYPE=Debug -DAXEL_TEST=YES'

cmake -S "$dir" -B "$dir"/build $cmake_args -DCMAKE_EXPORT_COMPILE_COMMANDS=1
