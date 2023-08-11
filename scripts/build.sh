#!/bin/bash
set -euo pipefail

echo ">> Building axel"

# Compile the latest version of the main.cpp
docker build -t axel -f axel.Dockerfile --no-cache .
echo ">> Compiled: main"

# Create a temporary container to copy
docker create --name temp axel > /dev/null 2>&1
echo ">> Container created: temp"

# Copy out the zip
docker cp temp:/app/axel/build/main.zip . > /dev/null 2>&1
echo ">> Zip copied to: $(pwd)"

# Remove the container
docker rm temp > /dev/null 2>&1
echo ">> Temporary container removed"