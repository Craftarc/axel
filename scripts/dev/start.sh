#!/bin/bash

echo ">> Inside container"
set -euo pipefail

# Set CMake arguments here
cmake_args='-G Ninja -DCMAKE_BUILD_TYPE=Debug -DAXEL_TEST=YES'

# Cleanup function that write logs to mounted project root
signal_handler() {
  echo -e ">> Caught a signal. Cleaning up..."
  echo ">> Stopping RIE with PID: ${rie_pid}"
  kill -TERM "${rie_pid}"

  logfile=${cwd}/logs/"$(date +"%Y-%m-%dT%H:%M:%SZ")".log
  grep -e '\[[0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}[[:space:]][0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\}\][[:space:]]\[[a-z]\+\]' "${cwd}"/logs/tmp.log > "${logfile}" || true
  rm "${cwd}"/logs/tmp.log

  echo ">> Logs cleaned up. Logfile at: ${logfile}"

  exit 0
}

trap 'signal_handler' EXIT

cwd=$(pwd)
echo ">> Current working directory: ${cwd}"

# Make the logs directory if it does not exist
if [ ! -d ./logs ]
then
  echo "Directory 'logs' does not exist. Creating..."
  mkdir logs
  echo "Directory 'logs' created"
else
  echo "Directory 'logs' already exists. Skipping mkdir..."
fi

# Make the build directory if it does not exist
if [ ! -d ./build ]
then
  echo "Directory 'build' does not exist. Creating..."
  mkdir build
  echo "Directory 'build' created"
else
  echo "Directory 'build' already exists. Skipping mkdir..."
fi

# Build updated executables

# Compile and build
mkdir -p build
cmake -B build -S . ${cmake_args} # CMake configuration setup
cmake --build build --target all --target aws-lambda-package-main
echo ">> Compilation complete"


# Start RIE in background
echo ">> Starting RIE and invoking the handler..."
/aws-lambda/aws-lambda-rie  "${cwd}"/build/main > logs/tmp.log 2>&1 &

# Save PID of RIE
rie_pid=$!

# Get progress updates by printing out the tail end of logs
tail -f "${cwd}"/logs/tmp.log



