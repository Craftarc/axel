#!/bin/bash

echo "Building axel"

# Compile the latest version of the main.cpp
docker build -t axel-backend -f axel.Dockerfile . > /dev/null 2>&1
echo "compiled: main"

# Create a temporary container to copy
docker create --name temp axel-backend > /dev/null 2>&1
echo "Container created: temp"

# Copy out the zip
docker cp temp:/app/axel/build/main.zip . > /dev/null 2>&1
echo "Zip copied to: $(pwd)"

# Remove the container
docker rm temp > /dev/null 2>&1