#!/bin/bash
set -euo pipefail

# Set CMake flags here
cmake_arguments="-DCMAKE_BUILD_TYPE=Debug -G Ninja -DTEST=YES"

# Get the container id currently running on port 9000
old_container_id=$(docker ps --format '{{.ID}}:{{.Ports}}' | grep ":9000" | cut -d ":" -f 1)
echo ">> Old container found, with id: ${old_container_id}"

# Kill it
docker kill "${old_container_id}" > /dev/null
echo ">> Killed container: ${old_container_id}"

# Start the container and mount the project root and RIE
echo ">> Starting new container..."
container_id=$(docker run -d -i \
-v ~/.aws-lambda-rie:/aws-lambda \
-v ~/dev/axel:/app/axel \
-v ~/.aws:/root/.aws \
-p 9000:8080 \
-e AWS_LAMBDA_RUNTIME_API=localhost:9000 \
dependencies)
echo ">> New container started, with id: ${container_id}"

# Build updated executables
docker exec "${container_id}" \
sh -c "cd /app/axel/build && cmake .. ${cmake_arguments} && ninja"
echo ">> Updated executables built"

# Start the RIE and pass the handler in
echo ">> Starting RIE..."
docker exec "${container_id}" \
sh -c "/aws-lambda/aws-lambda-rie /app/axel/build/main"

